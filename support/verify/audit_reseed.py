"""重播全部随机测试，额外扰动随机种子；只在 build 中生成副本与证据。"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
RNG = re.compile(r"((?:std::)?mt19937(?:_64)?\s+\w+\()([^;\n]+?)(\);)")


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--salt", type=lambda s: int(s, 0), default=2026101202)
    parser.add_argument("--only", choices=[p.stem for p in (ROOT / "support/verify").glob("*random.cpp")])
    args = parser.parse_args()
    import resource
    resource.setrlimit(resource.RLIMIT_STACK, (256 * 1024 * 1024, resource.RLIM_INFINITY))
    suffix = "-" + args.only if args.only else ""
    out = ROOT / "build/audit-20261012" / f"reseed-{args.salt}{suffix}"
    out.mkdir(parents=True, exist_ok=True)
    snapshots = {str(p.relative_to(ROOT)): digest(p) for p in sorted((ROOT / "template").glob("*/*.cpp"))}
    tests = sorted((ROOT / "support/verify").glob("*random.cpp"))
    if args.only:
        tests = [p for p in tests if p.stem == args.only]
    results = {"salt": args.salt, "stack_bytes": 256 * 1024 * 1024,
               "template_sha256": snapshots, "runs": []}
    for path in tests:
        text = path.read_text(encoding="utf-8")
        modified, count = RNG.subn(lambda m: m[1] + f"(({m[2]}) ^ {args.salt}ULL)" + m[3], text)
        assert count, f"没有识别到随机种子：{path}"
        modified = re.sub(r'#include "(../../[^"]+)"',
                          lambda m: '#include "' + (path.parent / m[1]).resolve().as_posix() + '"', modified)
        copy = out / path.name
        copy.write_text(modified, encoding="utf-8")
        binary = out / path.stem
        cmd = ["g++", "-std=c++17", "-O1", "-g", "-Wall", "-Wextra", "-pedantic",
               "-D_GLIBCXX_ASSERTIONS", "-fsanitize=address,undefined", "-fno-sanitize-recover=all",
               str(copy), "-o", str(binary)]
        record = {"test": path.name, "sha256": digest(path), "rng_constructors": count,
                  "compile_command": cmd}
        started = time.monotonic()
        print(f"RUN salt={args.salt} {path.name}", flush=True)
        with (out / (path.stem + ".log")).open("w", encoding="utf-8") as log:
            try:
                compiled = subprocess.run(cmd, cwd=ROOT, stdout=log, stderr=log, timeout=240)
                record["compile_exit"] = compiled.returncode
                if compiled.returncode == 0:
                    run = subprocess.run([str(binary)], cwd=ROOT, stdout=log, stderr=log, timeout=300)
                    record["run_exit"] = run.returncode
                    record["status"] = "pass" if run.returncode == 0 else "fail"
                else:
                    record["status"] = "compile_fail"
            except subprocess.TimeoutExpired:
                record["status"] = "timeout"
        record["seconds"] = round(time.monotonic() - started, 3)
        results["runs"].append(record)
        (out / "results.json").write_text(json.dumps(results, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(f"{record['status'].upper()} {path.name} {record['seconds']}s", flush=True)
    assert snapshots == {str(p.relative_to(ROOT)): digest(p) for p in sorted((ROOT / "template").glob("*/*.cpp"))}, "运行期间模板发生变化"
    passed = sum(r["status"] == "pass" for r in results["runs"])
    status = "PASS" if passed == len(tests) else "FAIL"
    print(f"{status} {passed}/{len(tests)} random programs, salt={args.salt}", flush=True)
    raise SystemExit(0 if passed == len(tests) else 1)


if __name__ == "__main__":
    main()
