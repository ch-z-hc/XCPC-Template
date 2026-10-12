#include "../../template/geometry/点与向量.cpp"
#include "../../template/geometry/直线与线段.cpp"
#include "../../template/geometry/圆.cpp"
#include "../../template/geometry/圆面积并与周长并.cpp"
#include <cassert>

const db pi = acosl(-1);

void close(db a, db b, db eps = 1e-9L) {
    if (fabsl(a - b) > eps * max(1.L, fabsl(b))) {
        cerr << setprecision(18) << a << " != " << b << '\n'; abort();
    }
}

// 独立竖切片积分，不使用圆弧、交点或 Green 公式。
db numerical_area(const V < Circle > &cs) {
    if (cs.empty()) return 0;
    db l = 1e100L, r = -1e100L;
    for (Circle c : cs) {
        l = min(l, c.o.x - c.r); r = max(r, c.o.x + c.r);
    }
    const int steps = 60000;
    db ans = 0;
    for (int k = 0; k < steps; ++ k) {
        db x = l + (r - l) * (k + 0.5L) / steps;
        V < pair < db, db > > seg;
        for (Circle c : cs) {
            db t = c.r * c.r - (x - c.o.x) * (x - c.o.x);
            if (t <= 0) continue;
            db h = sqrtl(t); seg.push_back({c.o.y - h, c.o.y + h});
        }
        sort(seg.begin(), seg.end());
        db top = -1e100L, len = 0;
        for (auto [a, b] : seg) {
            len += max(0.L, b - max(a, top)); top = max(top, b);
        }
        ans += len;
    }
    return ans * (r - l) / steps;
}

// 独立等距圆周采样；每次只判断样点是否落入其他圆内部。
db numerical_perimeter(V < Circle > cs) {
    if (! cs.empty()) {
        P origin = cs[0].o;
        for (Circle &c : cs) c.o = c.o - origin;
    }
    const int steps = 24000;
    db ans = 0;
    for (int i = 0; i < (int)cs.size(); ++ i) {
        Circle c = cs[i];
        if (c.r == 0) continue;
        bool duplicate = 0;
        for (int j = 0; j < i; ++ j)
            if (c.r == cs[j].r && c.o.x == cs[j].o.x &&
                c.o.y == cs[j].o.y) duplicate = 1;
        if (duplicate) continue;
        int cnt = 0;
        for (int k = 0; k < steps; ++ k) {
            db t = 2 * pi * (k + 0.5L) / steps;
            P p = c.o + P(cosl(t), sinl(t)) * c.r;
            bool covered = 0;
            for (int j = 0; j < (int)cs.size(); ++ j) {
                if (i == j) continue;
                covered |= (p - cs[j].o).abs2() <
                           cs[j].r * cs[j].r - 1e-13L;
            }
            cnt += ! covered;
        }
        ans += 2 * pi * c.r * cnt / steps;
    }
    return ans;
}

void check(V < Circle > cs, db a, db p) {
    auto [area, perimeter] = circle_union(cs);
    close(area, a); close(perimeter, p);
}

int main() {
    check({}, 0, 0);
    check({{{1, 2}, 0}}, 0, 0);
    check({{{1, 2}, 2}}, 4 * pi, 4 * pi);
    check({{{0, 0}, 1}, {{0, 0}, 1}}, pi, 2 * pi);
    check({{{0, 0}, 2}, {{0.5L, 0}, 1}}, 4 * pi, 4 * pi);
    check({{{0, 0}, 2}, {{1, 0}, 1}}, 4 * pi, 4 * pi);
    check({{{0, 0}, 1}, {{2, 0}, 1}}, 2 * pi, 4 * pi);
    check({{{0, 0}, 1}, {{3, 0}, 1}}, 2 * pi, 4 * pi);
    check({{{0, 0}, 1}, {{1, 0}, 1}},
          4 * pi / 3 + sqrtl(3) / 2, 8 * pi / 3);
    check({{{0, 0}, 1}, {{-1, 0}, 1}},
          4 * pi / 3 + sqrtl(3) / 2, 8 * pi / 3);
    // 比 EPS 更小的正圆仍然保留，近重合圆不应被双向删除。
    check({{{0, 0}, 1e-12L}}, pi * 1e-24L, 2 * pi * 1e-12L);
    auto near = circle_union({{{0, 0}, 1}, {{1e-10L, 0}, 1}});
    assert(near.first > 3 && near.second > 6);
    for (unsigned seed : {712311u, 2654840782u}) {
        mt19937 rng(seed);
        for (int tc = 0; tc < 100; ++ tc) {
            V < Circle > cs;
            int n = 1 + rng() % 6;
            for (int i = 0; i < n; ++ i)
                cs.push_back({{(db)((int)(rng() % 17) - 8) / 3,
                               (db)((int)(rng() % 17) - 8) / 3},
                              (db)(rng() % 9) / 3});
            if (tc == 0) {
                // 三圆包围一个洞，检验内边界积分的方向。
                cs = {{{0, 0}, 1}, {{1.8L, 0}, 1},
                      {{0.9L, 0.9L * sqrtl(3)}, 1}};
            }
            if (tc % 7 == 0) cs.push_back(cs[0]);
            auto [a, p] = circle_union(cs);
            close(a, numerical_area(cs), 2e-5L);
            db sum = 0;
            for (Circle c : cs) sum += c.r;
            db error = 4 * pi * cs.size() * sum / 24000 + 1e-9L;
            assert(fabsl(p - numerical_perimeter(cs)) <= error);
            shuffle(cs.begin(), cs.end(), rng);
            auto [b, q] = circle_union(cs); close(a, b); close(p, q);
            for (Circle &c : cs) c.o = c.o + P(1e6L, -1e6L);
            auto [c, r] = circle_union(cs); close(a, c);
            // 相切经坐标舍入后可能微交；校验平移后实际输入，而非强求周长不变。
            assert(fabsl(r - numerical_perimeter(cs)) <= error);
        }
    }
    cout << "circle_union_random OK: analytic cases and 200 numerical sets\n";
}
