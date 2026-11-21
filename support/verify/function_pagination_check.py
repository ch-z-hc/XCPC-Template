"""验证真实模板和 C++ 语法边界；分页不得拆开能完整放入一页的函数。"""
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from generate_print import (CODE_W, MIN_CODE_SIZE, TOP, code_boundaries,
                            load_entries, page_capacity, plan_code, split_rows)


def verify(source, path, top=TOP):
    rows = list(enumerate(source.splitlines(), 1))
    boundaries = code_boundaries(source, path)
    if not split_rows(rows, top, MIN_CODE_SIZE, boundaries):
        return
    plan = plan_code(rows, top, CODE_W * .9, boundaries)
    assert sum(plan.chunks) == len(rows)
    breaks, end = [], 0
    for count in plan.chunks[:-1]:
        end += count
        breaks.append(end)
    for start, end, function in boundaries:
        limit = page_capacity(TOP, MIN_CODE_SIZE if function else plan.font_size)
        if end - start <= limit:
            assert not any(start < point < end for point in breaks), (path, start, end, breaks)
    return plan, boundaries, breaks


def main():
    for entry in load_entries():
        for top in (TOP, TOP - 200):
            verify(entry.source.expandtabs(4), entry.path, top)
    fenwick = next(e for e in load_entries() if e.path == "template/ds/树状数组.cpp")
    _, spans, breaks = verify(fenwick.source, fenwick.path)
    lines = fenwick.source.splitlines()
    start = lines.index("    int kth(ll k) {")
    end = lines.index("    }", start + 1) + 1
    assert lines[start - 1].strip() == "*/"
    comment = max(i for i in range(start) if lines[i].strip() == "/*")
    assert breaks == [comment], breaks
    assert (start, end, True) in spans
    assert (comment, end, False) in spans
    source = '\n' * 12 + '''template <class T>
// { 不能作为函数体的开始。
T f(T x) {
    const char *s = R"tag( } { // )tag";
    auto inner = [x]() {
        return x;
    };
    return inner();
}
struct A {
    A() : n{1} {}
    int n;
    bool operator <(const A &a) const {
        return n < a.n;
    }
};
unsigned g() {
    return unsigned(1);
}
'''
    spans = code_boundaries(source, "syntax.cpp")
    assert (14, 21, True) in spans and (16, 19, True) in spans
    assert (24, 27, True) in spans and (28, 31, True) in spans
    # 单个函数超页时，在完整循环之间分开，保留所有行。
    source = '\n' * 12 + 'void large() {\n' + ''.join(
        '    for (int i = 0; i < 3; ++i) {\n' + '        use(i);\n' * 17 + '    }\n'
        for _ in range(5)) + '}\n'
    plan, spans, breaks = verify(source, "large.cpp")
    assert len(plan.chunks) > 1
    assert all(point in (12, 13, 32, 51, 70, 89, 108) for point in breaks), breaks
    print("PASS: all template function boundaries, attached comments, templates, lambdas, casts and oversized functions")


if __name__ == "__main__":
    main()
