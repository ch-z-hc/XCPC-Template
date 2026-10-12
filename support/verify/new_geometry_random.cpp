#include <cassert>
#include "../../template/geometry/点与向量.cpp"
#include "../../template/geometry/直线与线段.cpp"
#include "../../template/geometry/圆.cpp"
#include "../../template/geometry/半平面交.cpp"
#include "../../template/geometry/最小圆覆盖.cpp"

bool near(db a, db b, db eps = 1e-7L) {
    return fabsl(a - b) <= eps * max(1.L, max(fabsl(a), fabsl(b)));
}

db dist(P a, P b) { return hypotl(a.x - b.x, a.y - b.y); }

void test_circles(mt19937 &rng) {
    const db pi = acosl(-1);
    Circle a{{0, 0}, 1};
    assert(isCL(a, {-2, 0}, {2, 0}).size() == 2);
    assert(isCL(a, {-2, 1}, {2, 1}).size() == 1);
    assert(isCL(a, {-2, 2}, {2, 2}).empty());
    assert(isCC(a, {{2, 0}, 1}).size() == 1);
    assert(isCC(a, {{1, 0}, 1}).size() == 2);
    assert(isCC(a, {{3, 0}, 1}).empty());
    assert(isCC(a, a).empty());
    assert(isCC({{0, 0}, 0}, {{0, 0}, 0}).size() == 1);
    assert(isCC({{0, 0}, 0}, {{1, 0}, 0}).empty());
    assert(isCC({{1, 0}, 0}, a).size() == 1);
    assert(isCC({{0, 0}, 0}, a).empty());
    assert(isCC({{0, 0}, 2}, {{1, 0}, 1}).size() == 1);
    assert(isCC({{0, 0}, 3}, {{1, 0}, 1}).empty());
    assert(tangent_points(a, {0, 0}).empty());
    assert(tangent_points(a, {1, 0}).size() == 1);
    assert(tangent_points(a, {2, 0}).size() == 2);
    assert(tangent_points({{0, 0}, 0}, {2, 0}).size() == 1);
    assert(common_tangents(a, {{3, 0}, 1}).size() == 4);
    assert(common_tangents(a, {{2, 0}, 1}).size() == 3);
    assert(common_tangents(a, {{1, 0}, 1}).size() == 2);
    assert(common_tangents({{0, 0}, 2}, {{1, 0}, 1}).size() == 1);
    assert(common_tangents({{0, 0}, 3}, {{1, 0}, 1}).empty());
    assert(common_tangents(a, a).empty());
    assert(near(circle_area(a, a), pi));
    assert(near(circle_area(a, {{3, 0}, 1}), 0));
    assert(near(circle_area(a, {{2, 0}, 1}), 0));
    assert(near(circle_area({{0, 0}, 2}, a), pi));
    assert(near(circle_area(a, {{1, 0}, 1}), 2 * pi / 3 - sqrtl(3) / 2));
    assert(near(circle_area(a, {{0, 0}, 0}), 0));
    db last = pi;
    for (int i = 0; i <= 30; ++ i) {
        db value = circle_area(a, {{i / 10.L, 0}, 1});
        assert(value <= last + 1e-9L); last = value;
    }
    Circle c;
    assert(! circumcircle({0, 0}, {1, 0}, {2, 0}, c));
    assert(circumcircle({0, 0}, {2, 0}, {0, 2}, c));
    assert(near(c.o.x, 1) && near(c.o.y, 1) && near(c.r, sqrtl(2)));
    auto d = diameter_circle({-2, 1}, {4, 1});
    assert(near(d.o.x, 1) && near(d.o.y, 1) && near(d.r, 3));

    for (int t = 0; t < 800; ++ t) {
        auto point = [&]() { return P(int(rng() % 41) - 20, int(rng() % 41) - 20); };
        Circle u{point(), (rng() % 40 + 1) / 4.L};
        Circle v{point(), (rng() % 40 + 1) / 4.L};
        auto pts = isCC(u, v), rev = isCC(v, u);
        assert(pts.size() == rev.size());
        for (P p : pts) {
            assert(near(dist(p, u.o), u.r) && near(dist(p, v.o), v.r));
            bool found = 0;
            for (P q : rev) if (dist(p, q) < 1e-6L) found = 1;
            assert(found);
        }
        P p = point(), q = point();
        if (dist(p, q) == 0) q.x += 1;
        Line line(p, q);
        for (P x : isCL(u, p, q)) {
            assert(near(dist(x, u.o), u.r));
            assert(fabsl(line.side(x)) < 1e-6L);
        }
        for (P x : tangent_points(u, p)) {
            assert(near(dist(x, u.o), u.r));
            db dot = (x.x - u.o.x) * (p.x - x.x);
            dot += (x.y - u.o.y) * (p.y - x.y);
            assert(fabsl(dot) < 1e-6L);
        }
        for (Line l : common_tangents(u, v)) {
            assert(near(fabsl(l.side(u.o)), u.r));
            assert(near(fabsl(l.side(v.o)), v.r));
        }
        db area = circle_area(u, v);
        assert(near(area, circle_area(v, u)));
        assert(area >= -1e-9L && area <= pi * min(u.r, v.r) * min(u.r, v.r) + 1e-7L);
    }
}

Circle brute_circle(const V < P > &a) {
    if (a.empty()) return {{0, 0}, 0};
    Circle best{{0, 0}, numeric_limits < db >::infinity()};
    auto check = [&](P o, db r) {
        if (r >= best.r) return;
        for (P p : a) if (dist(o, p) > r + 1e-8L) return;
        best = {o, r};
    };
    for (P p : a) check(p, 0);
    for (int i = 0; i < int(a.size()); ++ i)
        for (int j = i + 1; j < int(a.size()); ++ j) {
            P o((a[i].x + a[j].x) / 2, (a[i].y + a[j].y) / 2);
            check(o, dist(o, a[i]));
            for (int k = j + 1; k < int(a.size()); ++ k) {
                P p = a[i], q = a[j], r = a[k];
                db d = 2 * (p.x * (q.y - r.y) + q.x * (r.y - p.y)
                             + r.x * (p.y - q.y));
                if (fabsl(d) < 1e-15L) continue;
                db u = p.x * p.x + p.y * p.y;
                db v = q.x * q.x + q.y * q.y;
                db w = r.x * r.x + r.y * r.y;
                P o((u * (q.y - r.y) + v * (r.y - p.y) + w * (p.y - q.y)) / d,
                    (u * (r.x - q.x) + v * (p.x - r.x) + w * (q.x - p.x)) / d);
                check(o, dist(o, p));
            }
        }
    return best;
}

void test_min_circle(mt19937 &rng) {
    for (int t = 0; t < 700; ++ t) {
        int n = rng() % 10;
        V < P > a(n);
        for (P &p : a) {
            int x = int(rng() % 21) - 10, y = int(rng() % 21) - 10;
            p = t % 4 == 0 ? P(x, 2 * x + 3) : P(x, y);
        }
        if (n >= 2 && t % 3 == 0) a[1] = a[0];
        Circle ans = min_circle(a), expected = brute_circle(a);
        assert(near(ans.r, expected.r));
        for (P p : a) assert(dist(ans.o, p) <= ans.r + 1e-7L);
        if (a.empty()) assert(ans.o.x == 0 && ans.o.y == 0 && ans.r == 0);
    }
}

struct Half {
    db a, b, c;
    db value(P p) const { return c - a * p.x - b * p.y; }
    Line line() const {
        P p = a != 0 ? P(c / a, 0) : P(0, c / b);
        return Line(p, P(p.x - b, p.y + a));
    }
};

V < P > clip(V < P > a, Half h) {
    V < P > ans;
    for (int i = 0; i < int(a.size()); ++ i) {
        P p = a[i], q = a[(i + 1) % a.size()];
        db x = h.value(p), y = h.value(q);
        bool u = x >= -1e-12L, v = y >= -1e-12L;
        if (u) ans.push_back(p);
        if (u != v) {
            db t = x / (x - y);
            ans.emplace_back(p.x + (q.x - p.x) * t, p.y + (q.y - p.y) * t);
        }
    }
    return ans;
}

db polygon_area(const V < P > &a) {
    db sum = 0;
    for (int i = 0; i < int(a.size()); ++ i) {
        P p = a[i], q = a[(i + 1) % a.size()];
        sum += p.x * q.y - p.y * q.x;
    }
    return sum / 2;
}

void check_half(const V < Half > &h) {
    V < P > expected{{-10, -10}, {10, -10}, {10, 10}, {-10, 10}};
    V < Line > lines;
    for (Half x : h) { expected = clip(expected, x); lines.push_back(x.line()); }
    for (Half x : V < Half >{{1, 0, 10}, {-1, 0, 10}, {0, 1, 10}, {0, -1, 10}})
        lines.push_back(x.line());
    auto ans = half_plane(lines);
    db area = polygon_area(expected);
    if (area <= 1e-9L) assert(ans.empty());
    else {
        assert(ans.size() >= 3);
        assert(near(polygon_area(ans), area));
        for (P p : ans) {
            assert(fabsl(p.x) <= 10 + 1e-7L && fabsl(p.y) <= 10 + 1e-7L);
            for (Half x : h) assert(x.value(p) >= -1e-6L);
        }
    }
}

void test_half(mt19937 &rng) {
    check_half({});
    check_half({{1, 0, 3}, {1, 0, 3}, {2, 0, 6}, {1, 0, 5}});
    check_half({{1, 0, 3}, {-1, 0, -1}});
    check_half({{1, 0, 0}, {-1, 0, -1}});
    check_half({{1, 0, 0}, {-1, 0, 0}});
    check_half({{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}});
    assert(half_plane({}).empty());
    assert(half_plane({Line({0, 0}, {1, 0})}).empty());
    assert(half_plane({Line({0, 0}, {1, 0}), Line({1, 0}, {1, 1})}).empty());
    assert(half_plane({Line({0, 0}, {1, 0}), Line({1, 1}, {0, 1})}).empty());
    assert(half_plane({Line({0, 0}, {1, 0}), Line({1, 0}, {1, 1}),
                       Line({1, 0}, {0, 1})}).empty());
    for (int t = 0; t < 1500; ++ t) {
        V < Half > h;
        int n = rng() % 12;
        for (int i = 0; i < n; ++ i) {
            int a = int(rng() % 11) - 5, b = int(rng() % 11) - 5;
            if (! a && ! b) a = 1;
            h.push_back({static_cast<db> (a), static_cast<db> (b),
                         static_cast<db> (int(rng() % 61) - 30)});
            if (rng() % 4 == 0) h.push_back(h.back());
        }
        check_half(h);
    }
    // 同向重边可能因 signed zero 被 atan2 分到 -pi 和 +pi 两端。
    V < Line > lines{Line({0, -2}, {1, -2}), Line({1, -2}, {1, 0}),
                     Line({1, 0}, {0, 0}), Line({0, 0}, {0, -2}),
                     Line(P(1, 0), P(0, -0.L))};
    assert(near(polygon_area(half_plane(lines)), 2));
    // 近似同向重边横跨 0 / 2pi，同样应合并，不能误判成无界。
    lines = {Line({0, 0}, {1, 1e-12L}), Line({0, 0}, {1, -1e-12L}),
             Line({1, 0}, {1, 2}), Line({1, 2}, {0, 2}), Line({0, 2}, {0, 0})};
    assert(near(polygon_area(half_plane(lines)), 2));
}

int main() {
    mt19937 rng(114514);
    test_circles(rng); test_min_circle(rng); test_half(rng);
    cout << "PASS: extra circle, minimum circle and half-plane random tests\n";
}
