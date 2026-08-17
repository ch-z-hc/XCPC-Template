from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path


DIRECTORY = Path(__file__).resolve().parent
BUILD_DIRECTORY = DIRECTORY.parents[1] / "build" / "stress"
SOURCES = {
    "generator": DIRECTORY / "generator.cpp",
    "brute": DIRECTORY / "brute.cpp",
    "standard": DIRECTORY / "standard.cpp",
}


def executable_path(name: str) -> Path:
    suffix = ".exe" if os.name == "nt" else ""
    return BUILD_DIRECTORY / f"{name}{suffix}"


def compile_programs(compiler: str) -> dict[str, Path]:
    BUILD_DIRECTORY.mkdir(parents=True, exist_ok=True)
    executables: dict[str, Path] = {}
    for name, source in SOURCES.items():
        executable = executable_path(name)
        command = [
            compiler,
            "-std=c++17",
            "-O2",
            "-Wall",
            "-Wextra",
            "-pedantic",
            str(source),
            "-o",
            str(executable),
        ]
        subprocess.run(command, check=True)
        executables[name] = executable
    return executables


def run_program(command: list[str], input_data: bytes | None = None) -> bytes:
    result = subprocess.run(command, input=input_data, capture_output=True)
    if result.returncode != 0:
        sys.stderr.buffer.write(result.stderr)
        raise RuntimeError(f"程序异常退出，返回码为 {result.returncode}：{' '.join(command)}")
    return result.stdout


def save_failure(input_data: bytes, brute_output: bytes, standard_output: bytes) -> None:
    (BUILD_DIRECTORY / "failure.in").write_bytes(input_data)
    (BUILD_DIRECTORY / "failure.brute.out").write_bytes(brute_output)
    (BUILD_DIRECTORY / "failure.standard.out").write_bytes(standard_output)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="编译并运行 C++17 对拍示例")
    parser.add_argument("--tests", type=int, default=1000, help="测试轮数，默认 1000")
    parser.add_argument("--seed", type=int, default=1, help="首轮随机种子，默认 1")
    parser.add_argument("--compiler", default="g++", help="C++ 编译器，默认 g++")
    args = parser.parse_args()
    if args.tests <= 0:
        parser.error("--tests 必须是正整数")
    return args


def main() -> int:
    args = parse_args()
    try:
        executables = compile_programs(args.compiler)
        for test in range(args.tests):
            seed = args.seed + test
            input_data = run_program([str(executables["generator"]), str(seed)])
            brute_output = run_program([str(executables["brute"])], input_data)
            standard_output = run_program([str(executables["standard"])], input_data)
            if brute_output.split() != standard_output.split():
                save_failure(input_data, brute_output, standard_output)
                print(f"发现差异：第 {test + 1} 轮，种子 {seed}")
                print(f"失败数据和输出已保存到 {BUILD_DIRECTORY}")
                return 1
    except (OSError, subprocess.CalledProcessError, RuntimeError) as error:
        print(f"对拍失败：{error}", file=sys.stderr)
        return 2

    print(f"通过 {args.tests} 轮对拍，种子范围为 [{args.seed}, {args.seed + args.tests - 1}]")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
