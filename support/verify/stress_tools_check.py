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


def check_cpp():
    base = ROOT / "support/tools/stress"
    source = (base / "stress.cpp").read_text(encoding="utf-8")
    suffix = ".exe" if os.name == "nt" else ""
    flags = ["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic"]
    for name in ("gen", "sol", "bf", "stress"):
        subprocess.run([*flags, "-fsyntax-only", str(base / f"{name}.cpp")], check=True)
    with tempfile.TemporaryDirectory(prefix="cpp-stress-") as name:
        work = Path(name)
        program = lambda name: work / (name + suffix)

        def compile_file(source, target):
            subprocess.run([*flags, str(source), "-o", str(program(target))], check=True)

        def runner(first, last):
            # 只限制测试副本的轮数，正式程序仍从 1 开始持续运行。
            marker = "int i = 1; ;"
            assert source.count(marker) == 1
            (work / "stress.cpp").write_text(
                source.replace(marker, f"int i = {first}; i <= {last};"), encoding="utf-8")
            compile_file(work / "stress.cpp", "stress")

        def execute(mode=None, left=None, right=None):
            env = dict(os.environ)
            if mode is not None:
                env["STRESS_FIXTURE"] = mode
            if left is not None:
                env["STRESS_SOL_OUTPUT"] = left
            if right is not None:
                env["STRESS_BF_OUTPUT"] = right
            return subprocess.run([str(program("stress"))], cwd=work, env=env,
                                  capture_output=True, text=True, encoding="utf-8",
                                  errors="replace", timeout=60)

        def restore():
            for target in ("gen", "bf", "sol"):
                shutil.copy2(work / f"{target}.ok", program(target))

        for target in ("gen", "bf", "sol"):
            compile_file(base / f"{target}.cpp", target)
            shutil.copy2(program(target), work / f"{target}.ok")
        runner(1, 200)
        result = execute()
        assert result.returncode == 0 and result.stdout.splitlines() == ["AC 100", "AC 200"], result
        for seed in (1, 123, 200, 347):
            data = subprocess.check_output([str(program("gen")), str(seed)])
            assert data == subprocess.check_output([str(program("gen")), str(seed)])
            values = list(map(int, data.split()))
            assert 1 <= values[0] <= 10 and len(values) == values[0] + 1
            assert all(1 <= x <= 100 for x in values[1:])
        assert (work / "in.txt").read_bytes() == subprocess.check_output([str(program("gen")), "200"])
        print("PASS: C++ example, 200 rounds, AC cadence and deterministic valid seeds", flush=True)

        runner(347, 366)
        for case in stress.CASES:
            folder = base if case == "max_subarray" else base / "cases" / case
            for original, target in (("generator", "gen"), ("brute", "bf"), ("standard", "sol")):
                compile_file(folder / f"{original}.cpp", target)
            result = execute()
            assert result.returncode == 0 and result.stdout == "", result
            assert (work / "in.txt").read_bytes() == subprocess.check_output([str(program("gen")), "366"])
            print(f"PASS: C++ runner, {case}, 20 rounds", flush=True)

        runner(347, 347)
        restore()
        # 故意改测试副本的 sol.cpp，正式示例不受影响。
        (work / "sol.cpp").write_text(
            (base / "sol.cpp").read_text(encoding="utf-8").replace(
                "cout << ans", "cout << ans + 1"), encoding="utf-8")
        compile_file(work / "sol.cpp", "sol")
        result = execute()
        assert result.returncode == 1 and result.stdout == "WA seed=347\n", result
        assert (work / "in.txt").read_bytes() == subprocess.check_output([str(program("gen")), "347"])
        assert int((work / "out.txt").read_text()) == int((work / "ans.txt").read_text()) + 1
        print("PASS: modified sol.cpp detects WA, saved seed 347 reproduces input", flush=True)

        compile_file(ROOT / "support/verify/fixtures/stress_failure.cpp", "fixture")
        # 固定另一侧的答案，覆盖空白差异、两侧多余 token、空输出和精确比较。
        restore()
        shutil.copyfile(program("fixture"), program("sol"))
        shutil.copyfile(program("fixture"), program("bf"))
        for left, right, same in (("1 2\n", " 1\n\n2 \t", True),
                                  ("", " \n", True), ("", "1", False),
                                  ("1", "", False), ("1 2", "1", False),
                                  ("1", "1 2", False), ("1", "1.0", False)):
            result = execute("tokens", left, right)
            assert result.returncode == (0 if same else 1), (left, right, result)
            assert result.stdout == ("" if same else "WA seed=347\n"), result
        print("PASS: whitespace, empty outputs, extra tokens on either side and exact tokens", flush=True)

        for role in ("sol", "bf", "gen"):
            restore()
            shutil.copyfile(program("fixture"), program(role))
            result = execute("crash")
            assert result.returncode == 2 and f"RE {role} seed=347" in result.stderr, result
            assert "AC" not in result.stdout and "fixture stderr" in result.stderr, result
            output = {"gen": "in.txt", "sol": "out.txt", "bf": "ans.txt"}[role]
            assert (work / output).read_bytes() == b"partial"
            print(f"PASS: {role} nonzero exit, partial output retained", flush=True)
        restore()
        program("sol").unlink()
        result = execute()
        assert result.returncode == 2 and "RE sol seed=347" in result.stderr, result
        restore()
        shutil.copyfile(program("fixture"), program("bf"))
        result = execute("remove_output")
        assert result.returncode == 2 and "RE open out.txt seed=347" in result.stderr, result
        restore()
        (work / "in.txt").unlink()
        (work / "in.txt").mkdir()
        result = execute()
        assert result.returncode == 2 and "RE gen seed=347" in result.stderr, result
        print("PASS: missing executable, output open failure and redirection failure", flush=True)


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
        check_cpp()
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
    if sys.argv[1:] == ["--cpp-only"]:
        check_cpp()
    else:
        main()
