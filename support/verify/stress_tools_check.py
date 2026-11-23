"""验证对拍示例，以及错误答案、超时和异常退出的现场保存。"""
from __future__ import annotations

import contextlib
import importlib.util
import io
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "support/tools/stress/run.py"
spec = importlib.util.spec_from_file_location("stress", SCRIPT)
stress = importlib.util.module_from_spec(spec)
spec.loader.exec_module(stress)


def check_contest():
    base = ROOT / "support/tools/stress"
    flags = ["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic"]
    suffix = ".exe" if os.name == "nt" else ""
    with tempfile.TemporaryDirectory(prefix="contest-stress-") as directory:
        work = Path(directory)
        shutil.copyfile(base / "pai.sh", work / "pai.sh")

        def program(name):
            return work / (name + suffix)

        def compile_file(source, target):
            subprocess.run([*flags, str(source), "-o", str(program(target))], check=True)

        # 在下一轮让生成器返回 42，原样运行比赛脚本也能测试连续通过的轮次。
        gen = work / "gen"
        gen.write_bytes(b'#!/bin/sh\n[ "$#" -eq 0 ] || exit 1\n'
                        b'read i < round\ni=$((i+1))\necho "$i" > round\n'
                        b'if [ "$i" -gt "$STRESS_ROUNDS" ]; then exit 42; fi\n'
                        b'if [ "$STRESS_SEEDED" = 1 ]; then set -- "$i"; fi\n'
                        b'exec ./case_gen "$@"\n')
        gen.chmod(0o755)

        def execute(rounds=1, **env):
            (work / "round").write_bytes(b"0\n")
            return subprocess.run(["bash", "pai.sh"], cwd=work,
                                  env={**os.environ, "STRESS_ROUNDS": str(rounds), **env},
                                  capture_output=True, text=True, errors="replace", timeout=60)

        def check_rounds(rounds, seeded=False):
            result = execute(rounds, STRESS_SEEDED="1" if seeded else "0")
            assert result.returncode == 42 and result.stdout == "", result
            assert int((work / "round").read_text()) == rounds + 1

        for source, target in (("gen", "case_gen"), ("sol", "sol"), ("bf", "bf")):
            compile_file(base / f"{source}.cpp", target)
        check_rounds(200)
        samples = {subprocess.check_output([str(program("case_gen"))]) for _ in range(20)}
        assert len(samples) > 1
        for data in samples:
            values = list(map(int, data.split()))
            assert 1 <= values[0] <= 10 and len(values) == values[0] + 1
            assert all(1 <= x <= 100 for x in values[1:])
        print("PASS: pai.sh, 200 rounds and varied valid inputs without arguments", flush=True)

        for case in stress.CASES:
            folder = base if case == "max_subarray" else base / "cases" / case
            for source, target in (("generator", "case_gen"), ("brute", "bf"), ("standard", "sol")):
                compile_file(folder / f"{source}.cpp", target)
            check_rounds(20, seeded=True)
            print(f"PASS: pai.sh, {case}, 20 rounds", flush=True)

        compile_file(base / "gen.cpp", "case_gen")
        compile_file(base / "bf.cpp", "bf")
        (work / "sol.cpp").write_text(
            (base / "sol.cpp").read_text(encoding="utf-8").replace("cout << ans", "cout << ans + 1"),
            encoding="utf-8")
        compile_file(work / "sol.cpp", "sol")
        result = execute()
        assert result.returncode == 1 and result.stdout, result
        data = (work / "in").read_bytes()
        for target, output in (("sol", "out"), ("bf", "ans")):
            assert subprocess.check_output([str(program(target))], input=data) == (work / output).read_bytes()
        assert int((work / "out").read_text()) == int((work / "ans").read_text()) + 1

        compile_file(ROOT / "support/verify/fixtures/stress_failure.cpp", "fixture")
        for role in ("sol", "bf"):
            shutil.copyfile(program("fixture"), program(role))
        for left, right, same in (("1 2\n", "1 2 \t\n", True), ("", "", True),
                                  ("1 2\n", "1\n2\n", False), ("1 2", "12", False),
                                  ("", "1", False), ("1", "", False), ("1", "1.0", False)):
            result = execute(STRESS_FIXTURE="tokens", STRESS_SOL_OUTPUT=left, STRESS_BF_OUTPUT=right)
            assert result.returncode == (42 if same else 1), (left, right, result)

        for source, role, output in (("gen", "case_gen", "in"), ("sol", "sol", "out"), ("bf", "bf", "ans")):
            shutil.copyfile(program("fixture"), program(role))
            result = execute(STRESS_FIXTURE="crash")
            assert result.returncode == 7 and "fixture stderr" in result.stderr, result
            assert (work / output).read_bytes() == b"partial"
            compile_file(base / f"{source}.cpp", role)
        shutil.copyfile(program("fixture"), program("bf"))
        assert execute(STRESS_FIXTURE="remove_output").returncode == 2
        program("sol").unlink()
        assert execute().returncode == 127
        (work / "in").unlink()
        (work / "in").mkdir()
        assert execute().returncode != 0
        print("PASS: retained counterexample, line comparison, crashes and file errors", flush=True)


def main():
    for case in stress.CASES:
        subprocess.run([sys.executable, str(SCRIPT), "--case", case, "--tests", "200"], check=True)
        gen = ROOT / "build/stress" / case / ("generator.exe" if os.name == "nt" else "generator")
        for seed in range(16):
            a = subprocess.check_output([str(gen), str(seed)])
            assert a == subprocess.check_output([str(gen), str(seed)]), (case, seed)
    assert stress.same_output(b"1 2\n", b"1\n2 ", None)
    assert not stress.same_output(b"1", b"1.0", None)
    assert stress.same_output(b"1.0 YES", b"1.00000001 YES", 1e-6)
    assert not stress.same_output(b"1.0 YES", b"1.01 NO", 1e-6)
    assert not stress.same_output(b"nan", b"nan", 1e-6)
    assert not stress.same_output(b"inf", b"inf", 1e-6)
    assert not stress.same_output(b"1 2", b"1", 1e-6)

    work = Path(tempfile.mkdtemp(prefix="tool-check-", dir=ROOT / "build/stress"))
    suffix = ".exe" if os.name == "nt" else ""
    fixture = work / ("fixture" + suffix)
    subprocess.run(["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
                    str(ROOT / "support/verify/fixtures/stress_failure.cpp"), "-o", str(fixture)], check=True)
    normal = {name: ROOT / "build/stress/max_subarray" / (name + suffix)
              for name in ("generator", "brute", "standard")}
    generated = subprocess.check_output([str(normal["generator"]), "123"])
    for role, mode in (("standard", "wrong"), ("standard", "crash"), ("standard", "timeout"),
                       ("brute", "crash"), ("generator", "crash"), ("generator", "timeout")):
        programs = dict(normal); programs[role] = fixture
        before = set(work.glob("max_subarray/failure-*"))
        with patch.object(stress, "BUILD_DIRECTORY", work), \
                patch.object(stress, "compile_programs", return_value=programs), \
                patch.object(sys, "argv", [str(SCRIPT), "--tests", "1", "--seed", "123", "--timeout", ".2"]), \
                patch.dict(os.environ, {"STRESS_FIXTURE": mode}), \
                contextlib.redirect_stdout(io.StringIO()):
            code = stress.main()
        assert code == (1 if mode == "wrong" else 2), (role, mode, code)
        failures = set(work.glob("max_subarray/failure-*")) - before
        assert len(failures) == 1
        saved = failures.pop()
        data = json.loads((saved / "failure.json").read_text(encoding="utf-8"))
        assert data["seed"] == 123 and "--seed 123 --tests 1" in data["reproduce"]
        assert (saved / f"{role}.err").read_bytes().splitlines() == [b"fixture stderr"]
        assert data["programs"][role]["timeout"] == (mode == "timeout")
        if role != "generator":
            assert (saved / "failure.in").read_bytes() == generated
        if mode == "timeout":
            assert (saved / f"{role}.out").read_bytes() == b"partial"
        elif mode == "crash":
            assert data["programs"][role]["returncode"] == 7
    if sys.platform.startswith("linux"):
        check_contest()
        shell = ROOT / "support/tools/stress/run.sh"
        result = subprocess.run(["bash", str(shell)], cwd=ROOT,
                                env={**os.environ, "TESTS": "20"}, capture_output=True, text=True,
                                check=True, timeout=30)
        assert "OK 20" in result.stdout
        for mode in ("wrong", "crash", "timeout"):
            result = subprocess.run(["bash", str(shell),
                                     str(ROOT / "support/tools/stress/generator.cpp"),
                                     str(ROOT / "support/tools/stress/brute.cpp"),
                                     str(ROOT / "support/verify/fixtures/stress_failure.cpp")],
                                    cwd=ROOT, env={**os.environ, "STRESS_FIXTURE": mode},
                                    capture_output=True, text=True, timeout=30)
            assert result.returncode == (1 if mode == "wrong" else 2), result.stdout
            saved = Path(result.stdout.splitlines()[0].split("：", 1)[1])
            assert (saved / "seed.txt").read_text().strip() == "1"
            assert (saved / "input.txt").read_bytes()
            assert (saved / "a.err").read_bytes().splitlines() == [b"fixture stderr"]
        print("PASS: Bash success / wrong answer / crash / timeout")
    print("PASS: 6 cases x 200 rounds, deterministic seeds, comparisons and 6 failure paths")


if __name__ == "__main__":
    if sys.argv[1:] in (["--contest-only"], ["--cpp-only"]):
        check_contest()
    else:
        main()
