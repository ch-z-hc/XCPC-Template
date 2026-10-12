"""直接提取四个框架内的注释示例，替换声明后编译并运行，防止示例失效。"""
from pathlib import Path
import re
import resource
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build/print-usage"


def run_example(path, body):
    source = (ROOT / path).read_text(encoding="utf-8")
    block = source.split("    // 示例维护：", 1)[1].split("    void add", 1)[0]
    members = "\n".join(line.replace("    // ", "    ", 1)
                        for line in block.splitlines() if line.startswith("    // "))
    pattern = r"    void add\(int \w+\);\n    void del\(int \w+\);\n    void (?:answer|query)\(int \w+\);"
    source, count = re.subn(pattern, lambda _: members, source)
    assert count == 1, path
    generated = OUT / Path(path).name
    generated.write_text(source + "\nint main() {\n" + body + "\n}\n", encoding="utf-8")
    executable = generated.with_suffix("")
    subprocess.run(["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
                    "-fsanitize=address,undefined", "-fno-sanitize-recover=all",
                    str(generated), "-o", str(executable)], check=True)
    subprocess.run([str(executable)], check=True)
    print(f"PASS: printed member example, {path}", flush=True)


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    for path in (ROOT / "template").glob("*/*.cpp"):
        assert not re.search(r"\b(?:tie|move)\s*\(", path.read_text(encoding="utf-8")), path
    print("PASS: contest snippets contain no tie or move calls", flush=True)
    for path in list((ROOT / "template").glob("*/*.cpp")) + [ROOT / "support/tools/contest_main.cpp"]:
        assert not re.search(r"__u?int128|\b(?:i128|u128|print128)\b",
                             path.read_text(encoding="utf-8")), path
    print("PASS: contest snippets and starter contain no wide integer types", flush=True)
    run_example("template/others/莫队.cpp", """
    Mo mo; int n = 5, m = 3;
    mo.a = {0, 1, 2, 1, 3, 2};
    V < Mo::Query > qs{{1, 3, 0}, {2, 5, 1}, {4, 4, 2}};
    for (int repeat = 0; repeat < 2; ++ repeat) {
        mo.cnt.assign(m + 1, 0); mo.kinds = 0; mo.ans.resize(qs.size());
        mo.solve(n, qs); assert(mo.ans == V < int > ({2, 3, 1}));
    }
    """)
    run_example("template/others/带修改莫队.cpp", """
    MoModify mo; mo.init({0, 1, 2, 1, 3, 2});
    mo.query(1, 5); mo.modify(3, 4); mo.query(1, 3);
    mo.modify(2, 1); mo.query(1, 3);
    for (int repeat = 0; repeat < 2; ++ repeat) {
        mo.cnt.assign(5, 0); mo.kinds = 0; mo.ans.resize(mo.qs.size());
        mo.solve(); assert(mo.ans == V < int > ({3, 3, 2}));
    }
    """)
    run_example("template/others/树上莫队.cpp", """
    MoTree mo(5); mo.add_edge(1, 2); mo.add_edge(1, 3);
    mo.add_edge(3, 4); mo.add_edge(3, 5); mo.build();
    mo.query(2, 4); mo.query(4, 5); mo.query(3, 3); mo.query(2, 5);
    mo.color = {0, 1, 2, 1, 3, 2};
    for (int repeat = 0; repeat < 2; ++ repeat) {
        mo.cnt.assign(4, 0); mo.kinds = 0; mo.ans.resize(mo.qs.size());
        mo.solve(); assert(mo.ans == V < int > ({3, 3, 1, 2}));
    }
    """)
    run_example("template/graph/树上启发式合并.cpp", """
    DSUOnTree dsu(5); dsu.add_edge(1, 2); dsu.add_edge(1, 3);
    dsu.add_edge(3, 4); dsu.add_edge(3, 5);
    dsu.color = {0, 1, 2, 1, 3, 2};
    dsu.cnt.assign(4, 0); dsu.kinds = 0; dsu.ans.resize(6);
    for (int repeat = 0; repeat < 2; ++ repeat) {
        dsu.build(1); assert(dsu.ans == V < int > ({0, 3, 1, 3, 1, 1}));
        assert(dsu.kinds == 0 && dsu.cnt == V < int > (4, 0));
    }
    """)
    entry = (ROOT / "support/tools/contest_main.cpp").read_text(encoding="utf-8")
    entry = entry.replace("void solve() {",
                          "void solve() { int x; cin >> x; cout << x << '\\n';", 1)
    path = OUT / "contest_entry.cpp"
    path.write_text(entry, encoding="utf-8")
    binary = path.with_suffix("")
    subprocess.run(["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
                    str(path), "-o", str(binary)], check=True)
    result = subprocess.run([str(binary)], input="37\n", capture_output=True, text=True, check=True)
    assert result.stdout == "37\n", result.stdout
    print("PASS: single-case entry preserves the first input value", flush=True)
    # 不支持的巨大密集区间应被前提断言拒绝，而不是先触发有符号溢出。
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    for mode in ("wide-domain", "supply-overflow", "cap-overflow", "cost-overflow"):
        rejected = subprocess.run([str(ROOT / "build/linux-verify/print_usability"), mode],
                                  capture_output=True, text=True)
        assert rejected.returncode != 0 and "Assertion" in rejected.stderr, (mode, rejected.stderr)
        assert "runtime error:" not in rejected.stderr, (mode, rejected.stderr)
        print(f"PASS: {mode} rejected before arithmetic overflow", flush=True)


if __name__ == "__main__":
    main()
