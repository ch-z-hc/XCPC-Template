"""Linux 离线运行题面转载样例；不是官方全量数据或在线 AC。"""
from __future__ import annotations

import json
import resource
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

# 官方题面确认输入输出约定。官方样例直抓受 302 / 503 阻碍，
# 下列样例从每题两份公开题面转载交叉确认，保留实际来源供复核。
# 样例原文内嵌，复跑不访问网络，不用模板计算的结果充当期望答案。
PROBLEMS = [
    {
        "id": "P4213",
        "official_url": "https://www.luogu.com.cn/problem/P4213",
        "sample_sources": [
            "https://www.cnblogs.com/whznfy/p/18859453",
            "https://www.cnblogs.com/zwfymqz/p/8542105.html",
        ],
        "samples": [
            ("6\n1\n2\n8\n13\n30\n2333\n",
             "1 1\n2 0\n22 -2\n58 -3\n278 -3\n1655470 2\n"),
        ],
    },
    {
        "id": "P4720",
        "official_url": "https://www.luogu.com.cn/problem/P4720",
        "sample_sources": [
            "https://www.cnblogs.com/gaojunonly1/p/10555455.html",
            "https://blog.51cto.com/u_15314204/3191588",
        ],
        "samples": [
            ("5 3 3\n", "1\n"),
            ("666 233 123456\n", "61728\n"),
        ],
    },
    {
        "id": "P5490",
        "official_url": "https://www.luogu.com.cn/problem/P5490",
        "sample_sources": [
            "https://www.cnblogs.com/littlehb/p/17654282.html",
            "https://www.cnblogs.com/onlyblues/p/17836031.html",
        ],
        "samples": [
            ("2\n100 100 200 200\n150 150 250 255\n", "18000\n"),
        ],
    },
    {
        "id": "P1856",
        "official_url": "https://www.luogu.com.cn/problem/P1856",
        "sample_sources": [
            "https://www.cnblogs.com/gaojunonly1/p/10325073.html",
            "https://www.cnblogs.com/BCOI/p/10423238.html",
        ],
        "samples": [
            ("7\n-15 0 5 10\n-5 8 20 25\n15 -4 24 14\n0 -6 16 4\n"
             "2 15 10 22\n30 10 36 20\n34 0 40 16\n", "228\n"),
        ],
    },
]


def main():
    soft, hard = resource.getrlimit(resource.RLIMIT_STACK)
    target = 256 * 1024 * 1024
    if hard != resource.RLIM_INFINITY:
        target = min(target, hard)
    resource.setrlimit(resource.RLIMIT_STACK, (target, hard))
    out = ROOT / "build/luogu-samples"
    out.mkdir(parents=True, exist_ok=True)
    binary = out / "luogu"
    subprocess.run([
        "g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
        str(ROOT / "support/verify/oj/luogu.cpp"), "-o", str(binary),
    ], check=True)
    report = {
        "kind": "mirrored_statement_samples",
        "online_submission": False,
        "official_full_data": False,
        "problems": [],
    }
    total = 0
    for problem in PROBLEMS:
        passed = []
        for number, (sample_input, expected) in enumerate(problem["samples"], 1):
            result = subprocess.run(
                [str(binary), problem["id"]], input=sample_input,
                text=True, capture_output=True, check=True, timeout=30,
            )
            assert result.stdout.split() == expected.split(), (
                problem["id"], number, expected, result.stdout, result.stderr,
            )
            passed.append(number)
            total += 1
            print(f"PASS {problem['id']} sample {number} (mirrored statement)", flush=True)
        report["problems"].append({
            "id": problem["id"],
            "official_url": problem["official_url"],
            "sample_sources": problem["sample_sources"],
            "passed_samples": passed,
        })
    (out / "results.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8",
    )
    print(f"PASS {len(PROBLEMS)} problems, {total} mirrored statement samples")


if __name__ == "__main__":
    main()
