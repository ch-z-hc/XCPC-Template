#include "../../template/geometry/Point.cpp"
#include "../../template/geometry/Line.cpp"
#include "../../template/geometry/Polygon.cpp"
#include "../../template/geometry/ConvexHull.cpp"
#include "../../template/geometry/MinRectangle.cpp"

db brute(V < P > h) {
    if (h.size() <= 2) return 0;
    db ans = numeric_limits < db >::infinity();
    for (int i = 0; i < int(h.size()); ++ i) {
        P e = h[(i + 1) % h.size()] - h[i];
        db lo = 0, hi = 0, bottom = 0, top = 0;
        for (P p : h) {
            P d = p - h[i];
            lo = min(lo, d.dot(e)); hi = max(hi, d.dot(e));
            bottom = min(bottom, e.det(d)); top = max(top, e.det(d));
        }
        ans = min(ans, (hi - lo) * (top - bottom) / e.abs2());
    }
    return ans;
}

void check(V < P > h) {
    auto rect = min_rectangle(h);
    db expected = brute(h);
    assert(abs(rect.area - expected) <= 1e-8L * max(1.L, expected));
    assert(rect.corners.size() == (h.empty() ? 0 : 4));
    if (h.size() <= 2) return;
    assert(abs(area(rect.corners) - rect.area) <= 1e-8L * max(1.L, expected));
    for (int i = 0; i < 4; ++ i) {
        P a = rect.corners[i], b = rect.corners[(i + 1) % 4];
        P c = rect.corners[(i + 2) % 4], e = b - a;
        assert(abs(e.dot(c - b)) <= 1e-8L * max(1.L, e.abs() * (c - b).abs()));
        for (P p : h) assert(e.det(p - a) >= -1e-7L * max(1.L, e.abs()));
    }
}

int main() {
    check({}); check({{2, 3}}); check({{-2, 1}, {3, 6}});
    check({{0, 0}, {3, 0}, {3, 2}, {0, 2}});
    for (unsigned seed : {101126u, 2654401215u}) {
        mt19937 rng(seed);
        for (int rep = 0; rep < 3000; ++ rep) {
            int n = 1 + int(rng() % 50);
            V < P > ps;
            for (int i = 0; i < n; ++ i)
                ps.push_back({(db)(int(rng() % 101) - 50), (db)(int(rng() % 101) - 50)});
            auto h = convexHull(ps);
            check(h);
            rotate(h.begin(), h.begin() + rng() % h.size(), h.end()); check(h);
            db angle = (db)(rng() % 1000) / 100, scale = (db)(1 + rng() % 20) / 3;
            for (P &p : h) p = p.rot(angle) * scale + P(1000000, -2000000);
            check(h);
        }
    }
    V < P > regular;
    for (int i = 0; i < 100000; ++ i) {
        db a = 2 * acosl(-1) * i / 100000;
        regular.push_back({cosl(a), sinl(a)});
    }
    auto rect = min_rectangle(regular);
    assert(abs(rect.area - 4) < 1e-7L);
    cout << "PASS: minimum rectangle, independent edge enumeration and corners\n";
}
