"""独立枚举三角形和简单四边形，检查包围点解法的合法性与最优性。

在项目根目录运行：python3 support/verify/enclose_point_check.py build/enclose_point
点集内部的点必能被一个三角形或相邻两个扇形组成的四边形严格包含，
所以枚举到四边形足以得到最优答案；外部和边界点不存在答案。
"""

import itertools
import random
import subprocess
import sys


def cross(a, b, c):
    return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0])


def on_segment(a, b, p):
    return (cross(a, b, p) == 0
            and min(a[0], b[0]) <= p[0] <= max(a[0], b[0])
            and min(a[1], b[1]) <= p[1] <= max(a[1], b[1]))


def intersects(a, b, c, d):
    x, y, z, w = cross(a, b, c), cross(a, b, d), cross(c, d, a), cross(c, d, b)
    if any((on_segment(a, b, c), on_segment(a, b, d),
            on_segment(c, d, a), on_segment(c, d, b))):
        return True
    return x * y < 0 and z * w < 0


def simple(a):
    n = len(a)
    if len(set(a)) != n:
        return False
    for i in range(n):
        u, v, w = a[i - 1], a[i], a[(i + 1) % n]
        # 相邻共线边只允许继续前进，不允许回折重叠。
        if cross(u, v, w) == 0:
            dot = (u[0] - v[0]) * (w[0] - v[0]) + (u[1] - v[1]) * (w[1] - v[1])
            if dot > 0:
                return False
        for j in range(i + 1, n):
            if j == i + 1 or (i == 0 and j == n - 1):
                continue
            if intersects(a[i], a[(i + 1) % n], a[j], a[(j + 1) % n]):
                return False
    return True


def inside(a, p):
    # 独立使用绕数法，不复用待测程序的扇形二分或三角形判定。
    winding = 0
    for u, v in zip(a, a[1:] + a[:1]):
        if on_segment(u, v, p):
            return False
        if u[1] <= p[1] < v[1] and cross(u, v, p) > 0:
            winding += 1
        if v[1] <= p[1] < u[1] and cross(u, v, p) < 0:
            winding -= 1
    return winding != 0


def polygons(points, size):
    for subset in itertools.combinations(points, size):
        # 固定首点，并去掉反向重复；四边形仍枚举所有本质不同的排列。
        for tail in itertools.permutations(subset[1:]):
            if tail[0] > tail[-1]:
                continue
            a = [subset[0], *tail]
            if simple(a):
                yield a


def cases():
    yield [(0, 4), (-3, 0), (-3, -3), (3, -3), (3, 0)], [(0, 0)]
    yield [(0, 4), (-3, 0), (3, 0)], [(0, 0)]
    yield [(0, 0), (4, 0), (0, 4)], [(1, 1), (2, 0), (0, 0), (5, 5), (0, -1)]
    yield [(-2, -2), (2, -2), (2, 2), (-2, 2)], [(0, 0), (1, 0), (1, 1), (3, 0)]
    yield [(-2, -2), (2, -2), (2, 2), (-2, 2), (0, -2)], [(0, 0)]
    yield [(-2, -2), (2, -2), (2, 2), (-2, 2), (1, 0)], [(0, 0)]
    yield [(0, 0), (4, 0), (0, 4), (1, 1)], [(2, 0)]
    yield [(-2, -2), (2, -2), (2, 2), (-2, 2), (0, 0), (1, 1)], [(0, 0)]
    yield [(0, 0), (12, 0), (18, 6), (12, 12), (0, 12), (3, 3)], [(12, 4)]
    yield [(i, 0) for i in range(8)], [(2, 0), (2, 1), (8, 0)]
    bound = 10**9
    yield [(-bound, -bound), (bound, -bound), (bound, bound), (-bound, bound),
           (bound - 1, bound - 2)], [(0, 0), (bound - 1, bound - 1), (bound, 0)]
    # 穷举 3 × 3 网格的所有至少含 3 点的子集，以及 5 × 5 查询网格。
    grid = list(itertools.product(range(-1, 2), repeat=2))
    queries = list(itertools.product(range(-2, 3), repeat=2))
    for n in range(3, 10):
        for points in itertools.combinations(grid, n):
            yield list(points), queries
    rng = random.Random(23596)
    grid = list(itertools.product(range(-5, 6), repeat=2))
    for _ in range(1200):
        points = rng.sample(grid, rng.randint(3, 9))
        queries = [rng.choice(grid) for _ in range(12)]
        yield points, queries


def main():
    data = list(cases())
    lines = [str(len(data))]
    for points, queries in data:
        lines.append(f"{len(points)} {len(queries)}")
        lines.extend(f"{x} {y}" for x, y in points + queries)
    result = subprocess.run([sys.argv[1]], input="\n".join(lines) + "\n",
                            text=True, capture_output=True, check=True)
    tokens = iter(map(int, result.stdout.split()))
    count = 0
    for case_id, (points, queries) in enumerate(data):
        candidates = {m: list(polygons(points, m)) for m in (3, 4)}
        for p in queries:
            want = next((m for m in (3, 4) if any(inside(a, p) for a in candidates[m])), -1)
            got = next(tokens)
            context = (case_id, points, p, want, got)
            assert got == want, context
            if got != -1:
                ids = [next(tokens) for _ in range(got)]
                assert len(set(ids)) == got and all(1 <= i <= len(points) for i in ids), context
                a = [points[i - 1] for i in ids]
                assert simple(a) and inside(a, p), (context, ids)
            count += 1
    assert next(tokens, None) is None, "多余输出"
    print(f"PASS: {len(data)} point sets, {count} queries; minimum size, IDs, simplicity, strict containment")


if __name__ == "__main__":
    main()
