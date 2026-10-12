"""核对逐模板题目索引、测试入口；可在线核对 Library Checker 官方题面。"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import urllib.request
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime, timezone, timedelta
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--network", action="store_true")
    args = parser.parse_args()
    document = (ROOT / "support/verify/SOURCES.md").read_text(encoding="utf-8")
    rows = re.findall(r"^\| `(template/[^`]+\.cpp)` \| (.*?) \| (.*?) \|$", document, re.M)
    files = {p.relative_to(ROOT).as_posix() for p in (ROOT / "template").glob("*/*.cpp")}
    assert all(re.fullmatch(r"[\u4e00-\u9fff]+", Path(path).stem) for path in files), "模板文件名须为中文"
    assert len(rows) == len({row[0] for row in rows}), "索引中出现重复文件"
    indexed = {row[0] for row in rows}
    assert indexed == files, {"missing": sorted(files - indexed), "extra": sorted(indexed - files)}
    tests = {}
    for path in sorted((ROOT / "support/verify").glob("*.cpp")):
        text = path.read_text(encoding="utf-8")
        if path.stem.endswith("_submission") or path.stem == "mcmf_submission_standalone":
            continue
        for source in re.findall(r'#include "../../(template/[^"\n]+\.cpp)"', text):
            tests.setdefault(source, []).append(path.name)
    report = {
        "checked_at": datetime.now(timezone(timedelta(hours=8))).isoformat(),
        "scope": "链接与测试入口索引；不代表题目通过或分支覆盖率",
        "templates": [],
        "network": [],
    }
    for source, links, note in rows:
        urls = re.findall(r"\]\((https://[^)]+)\)", links)
        assert urls or source == "template/others/常用定义.cpp", source
        assert tests.get(source), f"缺少本地测试入口：{source}"
        report["templates"].append({
            "file": source, "sha256": sha((ROOT / source).read_bytes()),
            "urls": urls, "note": note, "test_entrypoints": tests[source],
        })
    report["test_sha256"] = {
        path.name: sha(path.read_bytes()) for path in sorted((ROOT / "support/verify").glob("*.cpp"))
    }
    if args.network:
        repo = ROOT / "build/library-checker-problems"
        commit = subprocess.check_output(["git", "-C", str(repo), "rev-parse", "HEAD"], text=True).strip()
        report["library_checker_commit"] = commit
        tracked = subprocess.check_output(
            ["git", "-C", str(repo), "ls-tree", "-r", "--name-only", commit], text=True,
        ).splitlines()
        problems = {Path(path).parent.name: Path(path).parent.as_posix()
                    for path in tracked if path.endswith("/info.toml")}
        modes = sorted({url.rsplit("/", 1)[1] for row in report["templates"] for url in row["urls"]
                        if url.startswith("https://judge.yosupo.jp/problem/")})

        def check(mode):
            assert mode in problems, f"官方仓库中没有题目：{mode}"
            records = []
            for name in ("info.toml", "task.md"):
                relative = problems[mode] + "/" + name
                url = f"https://raw.githubusercontent.com/yosupo06/library-checker-problems/{commit}/{relative}"
                with urllib.request.urlopen(url, timeout=30) as response:
                    data = response.read()
                expected = subprocess.check_output(["git", "-C", str(repo), "show", f"{commit}:{relative}"])
                assert data == expected, f"线上题面不一致：{relative}"
                records.append({"url": url, "sha256": sha(data)})
            return {"problem": mode, "status": "verified", "files": records}

        with ThreadPoolExecutor(max_workers=4) as pool:
            for record in pool.map(check, modes):
                report["network"].append(record)
                print(f"VERIFIED official statement: {record['problem']}", flush=True)
    out = ROOT / "build/source-audit"
    out.mkdir(parents=True, exist_ok=True)
    (out / "results.json").write_text(json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(rows)} templates indexed, each has a local test entrypoint")
    print(f"Official statements verified online: {len(report['network'])}")


if __name__ == "__main__":
    main()
