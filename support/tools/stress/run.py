"""编译生成器、暴力和待测程序，在相同输入上对拍。"""
from __future__ import annotations

import argparse
import json
import math
import os
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

DIRECTORY = Path(__file__).resolve().parent
BUILD_DIRECTORY = DIRECTORY.parents[2] / "build/stress"
CASES = ("max_subarray", "range_sum", "inversions", "tree_path", "shortest_path", "kmp")


def compile_programs(sources, directory, compiler, sanitize):
    executables = {}
    for name, source in sources.items():
        executable = directory / (name + (".exe" if os.name == "nt" else ""))
        flags = ["-std=c++17", "-Wall", "-Wextra", "-pedantic"]
        if sanitize:
            flags += ["-O1", "-g", "-D_GLIBCXX_ASSERTIONS", "-fsanitize=address,undefined",
                      "-fno-omit-frame-pointer", "-fno-sanitize-recover=all"]
        else:
            flags += ["-O2"]
        subprocess.run([compiler, *flags, str(source), "-o", str(executable)], check=True)
        executables[name] = executable
    return executables


def run_program(command, input_data, timeout):
    try:
        result = subprocess.run(command, input=input_data, capture_output=True, timeout=timeout)
        return {"stdout": result.stdout, "stderr": result.stderr,
                "returncode": result.returncode, "timeout": False}
    except subprocess.TimeoutExpired as error:
        return {"stdout": error.stdout or b"", "stderr": error.stderr or b"",
                "returncode": None, "timeout": True}
    except OSError as error:
        return {"stdout": b"", "stderr": str(error).encode("utf-8"),
                "returncode": None, "timeout": False}


def same_output(a, b, eps):
    left, right = a.split(), b.split()
    if eps is None:
        return left == right
    if len(left) != len(right):
        return False
    for x, y in zip(left, right):
        try:
            u, v = float(x), float(y)
        except ValueError:
            if x != y:
                return False
        else:
            if not (math.isfinite(u) and math.isfinite(v) and
                    math.isclose(u, v, rel_tol=eps, abs_tol=eps)):
                return False
    return True


def save_failure(directory, input_data, results, metadata):
    target = Path(tempfile.mkdtemp(prefix=f"failure-{metadata['seed']}-", dir=directory))
    (target / "failure.in").write_bytes(input_data)
    metadata["programs"] = {}
    for name, result in results.items():
        (target / f"{name}.out").write_bytes(result["stdout"])
        (target / f"{name}.err").write_bytes(result["stderr"])
        metadata["programs"][name] = {key: result[key] for key in ("returncode", "timeout")}
    (target / "failure.json").write_text(
        json.dumps(metadata, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"现场：{target}")
    print(f"复现：{metadata['reproduce']}")


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--case", choices=CASES, default="max_subarray", help="内置示例")
    parser.add_argument("--generator", type=Path, help="替换生成器源码")
    parser.add_argument("--brute", type=Path, help="替换暴力源码")
    parser.add_argument("--standard", type=Path, help="替换待测源码，例如 a.cpp")
    parser.add_argument("--tests", type=int, default=1000, help="轮数，0 表示持续运行")
    parser.add_argument("--seed", type=int, default=1, help="首轮种子，后续逐轮加 1")
    parser.add_argument("--timeout", type=float, default=2, help="每个程序每轮的秒数限制")
    parser.add_argument("--eps", type=float, help="浮点比较的绝对 / 相对误差，默认精确比较 token")
    parser.add_argument("--sanitize", action="store_true", help="启用 ASan / UBSan，建议在 Linux 使用")
    parser.add_argument("--compiler", default="g++", help="C++ 编译器")
    args = parser.parse_args()
    if args.tests < 0 or not 0 <= args.seed < 2 ** 64:
        parser.error("--tests 必须非负，--seed 必须在 [0, 2^64) 内")
    if args.tests and args.seed + args.tests > 2 ** 64:
        parser.error("种子范围超出 uint64_t")
    if not math.isfinite(args.timeout) or args.timeout <= 0:
        parser.error("--timeout 必须为有限正数")
    if args.eps is not None and (not math.isfinite(args.eps) or args.eps <= 0):
        parser.error("--eps 必须为有限正数")
    return args


def main():
    args = parse_args()
    if sys.platform.startswith("linux"):
        import resource
        _, hard = resource.getrlimit(resource.RLIMIT_STACK)
        stack = 256 * 1024 * 1024
        resource.setrlimit(resource.RLIMIT_STACK,
                           (stack if hard == resource.RLIM_INFINITY else min(stack, hard), hard))
    source_dir = DIRECTORY if args.case == "max_subarray" else DIRECTORY / "cases" / args.case
    sources = {name: (getattr(args, name) or source_dir / f"{name}.cpp").resolve()
               for name in ("generator", "brute", "standard")}
    directory = BUILD_DIRECTORY / args.case
    directory.mkdir(parents=True, exist_ok=True)
    try:
        executables = compile_programs(sources, directory, args.compiler, args.sanitize)
        test = 0
        while not args.tests or test < args.tests:
            seed = args.seed + test
            if seed >= 2 ** 64:
                raise RuntimeError("种子已达到 uint64_t 上限")
            results = {}
            input_data = b""
            reason = ""
            for name in ("generator", "brute", "standard"):
                command = [str(executables[name])]
                if name == "generator":
                    command.append(str(seed))
                result = run_program(command, None if name == "generator" else input_data, args.timeout)
                results[name] = result
                if name == "generator":
                    input_data = result["stdout"]
                if result["timeout"] or result["returncode"] != 0:
                    reason = name + (" 超时" if result["timeout"] else f" 异常退出 ({result['returncode']})")
                    break
            if not reason and not same_output(results["brute"]["stdout"], results["standard"]["stdout"], args.eps):
                reason = "答案不同"
            if reason:
                print(f"第 {test + 1} 轮，种子 {seed}：{reason}")
                reproduce = shlex.join([sys.executable, str(Path(__file__).resolve()),
                                       *sys.argv[1:], "--seed", str(seed), "--tests", "1"])
                save_failure(directory, input_data, results, {
                    "case": args.case, "test": test + 1, "seed": seed, "reason": reason,
                    "cwd": str(Path.cwd()), "sources": {k: str(v) for k, v in sources.items()},
                    "reproduce": reproduce,
                })
                return 1 if reason == "答案不同" else 2
            test += 1
            if test % 100 == 0:
                print(f"通过 {test} 轮，当前种子 {seed}", flush=True)
    except KeyboardInterrupt:
        print("\n已停止对拍")
        return 130
    except (OSError, subprocess.CalledProcessError, RuntimeError) as error:
        print(f"对拍失败：{error}", file=sys.stderr)
        return 2
    print(f"通过 {test} 轮，种子范围 [{args.seed}, {args.seed + test - 1}]")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
