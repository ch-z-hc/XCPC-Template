"""在 Linux 上运行 Library Checker 官方生成数据；不是在线提交。"""
from __future__ import annotations

import argparse
import hashlib
import json
import resource
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PROBLEMS = [
    "data_structure/range_chmin_chmax_add_range_sum",
    "data_structure/range_affine_range_sum",
    "data_structure/point_add_rectangle_sum",
    "tree/dynamic_tree_vertex_add_path_sum",
    "graph/general_matching",
    "graph/directedmst",
    "graph/counting_spanning_tree_directed",
    "graph/counting_spanning_tree_undirected",
    "number_theory/sqrt_mod",
    "number_theory/sum_of_floor_of_linear",
    "linear_algebra/matrix_det",
    "polynomial/division_of_polynomials",
    "polynomial/pow_of_formal_power_series",
    "polynomial/sqrt_of_formal_power_series",
    "geo/closest_pair",
    "linear_algebra/inverse_matrix",
    "number_theory/primitive_root",
    "set_power_series/subset_convolution",
    "polynomial/multipoint_evaluation",
    "polynomial/polynomial_interpolation",
]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=ROOT / "build/library-checker-problems")
    parser.add_argument("--generate", action="store_true", help="先调用官方生成器")
    args = parser.parse_args()
    repo = args.repo.resolve()
    resource.setrlimit(resource.RLIMIT_STACK, (256 * 1024 * 1024, resource.RLIM_INFINITY))
    out = ROOT / "build/public-verify"
    out.mkdir(parents=True, exist_ok=True)
    binary = out / "library_checker"
    subprocess.run(["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
                    str(ROOT / "support/verify/oj/library_checker.cpp"), "-o", str(binary)], check=True)
    commit = subprocess.check_output(["git", "-C", str(repo), "rev-parse", "HEAD"], text=True).strip()
    report = {"source": "https://github.com/yosupo06/library-checker-problems",
              "commit": commit, "online_submission": False, "problems": []}
    for relative in PROBLEMS:
        problem = repo / relative
        mode = problem.name
        if args.generate:
            subprocess.run(["python3", str(repo / "generate.py"), str(problem / "info.toml")],
                           cwd=repo, check=True)
        inputs = sorted((problem / "in").glob("*.in"))
        assert inputs, f"缺少官方数据：{relative}，先使用 --generate"
        hashes = json.loads((problem / "hash.json").read_text())
        assert {case.name for case in inputs} == {name for name in hashes if name.endswith(".in")}
        for name, expected_hash in hashes.items():
            folder = "in" if name.endswith(".in") else "out"
            assert hashlib.sha256((problem / folder / name).read_bytes()).hexdigest() == expected_hash, name
        passed = []
        for case in inputs:
            expected = problem / "out" / (case.stem + ".out")
            actual = out / (mode + ".out")
            with case.open("rb") as inp, actual.open("wb") as ans:
                subprocess.run([str(binary), mode], stdin=inp, stdout=ans, check=True, timeout=120)
            if mode == "directedmst":
                # 模板仅返回最小权，不恢复父亲；这里只对官方答案的最小权。
                assert actual.read_text().split()[0] == expected.read_text().split()[0], case
            else:
                checked = subprocess.run([str(problem / "checker"), str(case), str(actual), str(expected)],
                                         capture_output=True, text=True, timeout=120)
                assert checked.returncode == 0, (case, checked.stdout, checked.stderr)
            passed.append(case.name)
        report["problems"].append({"name": mode, "cases": passed,
                                   "check": "weight_only" if mode == "directedmst" else "official_checker"})
        (out / "results.json").write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")
        print(f"PASS {mode}: {len(passed)} official cases", flush=True)
    print(f"PASS {len(PROBLEMS)} problems, {sum(len(p['cases']) for p in report['problems'])} cases")


if __name__ == "__main__":
    main()
