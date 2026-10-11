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
    source = (ROOT / "support/tools/stress/stress.cpp").read_text(encoding="utf-8")
    subprocess.run(["g++", "-std=c++17", "-Wall", "-Wextra", "-pedantic",
                    "-fsyntax-only", str(ROOT / "support/tools/stress/stress.cpp")], check=True)
    with tempfile.TemporaryDirectory(prefix="cpp-stress-") as name:
        work = Path(name)
        # 仅让测试副本跑种子 123..142，不改正式程序的持续对拍写法。
        (work / "stress.cpp").write_text(
            source.replace("int t = 1; ;", "int t = 123; t <= 142;"), encoding="utf-8")
        subprocess.run(["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
                        "stress.cpp", "-o", "stress"], cwd=work, check=True)
        base = ROOT / "support/tools/stress"
        for case in stress.CASES:
            folder = base if case == "max_subarray" else base / "cases" / case
            for original, target in (("generator", "gen"), ("brute", "brute"), ("standard", "a")):
                subprocess.run(["g++", "-std=c++17", "-O2", str(folder / f"{original}.cpp"),
                                "-o", str(work / target)], check=True)
                if case == "max_subarray":
                    shutil.copyfile(work / target, work / f"{target}.ok")
            result = subprocess.run(["./stress"], cwd=work, capture_output=True, text=True, timeout=30)
            assert result.returncode == 0 and "Test 142" in result.stdout, result
            assert result.stdout.count("AC\n") == 20, result
            assert (work / "input.txt").read_bytes() == subprocess.check_output([str(work / "gen"), "142"])
            print(f"PASS: C++ runner, {case}, 20 rounds", flush=True)
        subprocess.run(["g++", "-std=c++17", "-O2",
                        str(ROOT / "support/verify/fixtures/stress_failure.cpp"),
                        "-o", str(work / "fixture")], check=True)
        for role, mode in (("a", "wrong"), ("a", "crash"), ("brute", "crash"), ("gen", "crash")):
            for target in ("gen", "brute", "a"):
                shutil.copyfile(work / f"{target}.ok", work / target)
            shutil.copyfile(work / "fixture", work / role)
            result = subprocess.run(["./stress"], cwd=work, capture_output=True, text=True, timeout=30,
                                    env={**os.environ, "STRESS_FIXTURE": mode})
            assert result.returncode == (1 if mode == "wrong" else 2), result
            assert "Test 123" in result.stdout and "AC\n" not in result.stdout
            assert "fixture stderr" in result.stderr, result
            if mode == "wrong":
                assert "WA\n" in result.stdout
            if role != "gen":
                expected = subprocess.check_output([str(work / "gen"), "123"])
                assert (work / "input.txt").read_bytes() == expected
            print(f"PASS: C++ runner, {role} / {mode}", flush=True)
        for target in ("gen", "brute", "a"):
            shutil.copyfile(work / f"{target}.ok", work / target)
        (work / "a").unlink()
        result = subprocess.run(["./stress"], cwd=work, capture_output=True, text=True, timeout=30)
        assert result.returncode == 2 and "AC\n" not in result.stdout, result
        print("PASS: C++ runner, missing executable", flush=True)


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
