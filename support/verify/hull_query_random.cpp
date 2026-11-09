#include "../../template/geometry/二维凸包最大点积查询.cpp"
#include "../../template/geometry/二维动态凸包.cpp"

using Wide = __int128_t;
using Point = HullQuery::P;

Wide det(Point a, Point b, Point c) {
    return (Wide(b.x) - a.x) * (Wide(c.y) - a.y) -
           (Wide(b.y) - a.y) * (Wide(c.x) - a.x);
}

// 枚举所有支撑直线，不使用被测凸包或二分。
int brute_contains(const V < Point > &p, Point q) {
    if (p.empty()) return 0;
    ll lx = p[0].x, rx = lx, ly = p[0].y, ry = ly;
    for (Point a : p) {
        lx = min(lx, a.x); rx = max(rx, a.x);
        ly = min(ly, a.y); ry = max(ry, a.y);
    }
    if (q.x < lx || q.x > rx || q.y < ly || q.y > ry) return 0;
    bool boundary = false, edge = false;
    for (Point a : p) for (Point b : p) {
        if (a == b) continue;
        bool support = true;
        for (Point c : p) if (det(a, b, c) < 0) support = false;
        if (! support) continue;
        edge = true;
        Wide d = det(a, b, q);
        if (d < 0) return 0;
        boundary |= d == 0;
    }
    return ! edge || boundary ? 1 : 2;
}

template < class T >
void check_query(T &hull, const V < Point > &p, ll a, ll b) {
    ll ans = 123456789;
    assert(hull.query(a, b, ans) == ! p.empty());
    if (p.empty()) return;
    Wide best = Wide(a) * p[0].x + Wide(b) * p[0].y;
    for (Point q : p) best = max(best, Wide(a) * q.x + Wide(b) * q.y);
    assert(Wide(ans) == best);
}

void check_hull(HullQuery &hull, const V < Point > &p) {
    for (Point a : hull.h)
        assert(find(p.begin(), p.end(), a) != p.end());
    int n = hull.h.size();
    for (int i = 0; i < n && n > 2; ++ i) {
        Point a = hull.h[i], b = hull.h[(i + 1) % n];
        assert(det(a, b, hull.h[(i + 2) % n]) > 0);
        for (Point c : p) assert(det(a, b, c) >= 0);
    }
}

int main(int argc, char **argv) {
    unsigned seed = argc > 1 ? stoul(argv[1]) : 20261109;
    mt19937 rng(seed);
    HullQuery hull;
    for (int mask = 0; mask < 512; ++ mask) {
        V < Point > p;
        for (int i = 0; i < 9; ++ i)
            if (mask >> i & 1) p.push_back({i / 3 - 1, i % 3 - 1});
        hull.build(p); check_hull(hull, p);
        for (int a = -4; a <= 4; ++ a)
            for (int b = -4; b <= 4; ++ b) check_query(hull, p, a, b);
        for (int x = -2; x <= 2; ++ x)
            for (int y = -2; y <= 2; ++ y)
                assert(hull.contains({x, y}) == brute_contains(p, {x, y}));
    }
    for (int tc = 0; tc < 2500; ++ tc) {
        V < Point > p;
        int n = rng() % 65;
        for (int i = 0; i < n; ++ i)
            p.push_back({(int)(rng() % 41) - 20, (int)(rng() % 41) - 20});
        if (tc % 4 == 0) for (auto &q : p) q.y = 2 * q.x + 1;
        if (tc % 4 == 1) for (auto &q : p) q.x = 7;
        shuffle(p.begin(), p.end(), rng);
        hull.build(p); check_hull(hull, p);
        for (int j = 0; j < 100; ++ j)
            check_query(hull, p, (int)(rng() % 101) - 50, (int)(rng() % 101) - 50);
        Point q{(int)(rng() % 51) - 25, (int)(rng() % 51) - 25};
        assert(hull.contains(q) == brute_contains(p, q));
    }
    V < Point > large{{-1000000000, -1000000000}, {1000000000, -1000000000},
                     {1000000000, 1000000000}, {-1000000000, 1000000000},
                     {999999999, 999999998}, {999999998, 999999997}};
    hull.build(large); check_hull(hull, large);
    for (ll a : {-1000000000ll, -1ll, 0ll, 1ll, 1000000000ll})
        for (ll b : {-1000000000ll, -1ll, 0ll, 1ll, 1000000000ll})
            check_query(hull, large, a, b);
    // 叉积只有 1 的大坐标三角形，不能按浮点容差丢掉极点。
    large = {{0, 0}, {999999999, 999999998}, {1000000000, 999999999}};
    hull.build(large); assert(hull.h.size() == 3);
    check_query(hull, large, -999999999, 1000000000);
    check_query(hull, large, 999999998, -999999999);

    DynamicHull dyn(2);
    int l = dyn.add({-2, 0}), r = dyn.add({2, 0});
    assert(dyn.contains({0, 0}) == 1); // 跨块凸包，不是块凸包的并。
    int inside = dyn.add({0, 0}), duplicate = dyn.add({0, 0});
    assert(dyn.erase(l) && dyn.erase(r));
    assert(dyn.contains({0, 0}) == 1);
    assert(dyn.erase(inside) && dyn.contains({0, 0}) == 1);
    assert(dyn.erase(duplicate));
    assert(! dyn.erase(duplicate) && ! dyn.erase(-1) && ! dyn.erase(100));
    check_query(dyn, {}, 0, 0);
    dyn.init(8);
    int outer = dyn.add({3, 0}); dyn.add({2, 0}); dyn.add({0, 0});
    assert(dyn.erase(outer));
    check_query(dyn, {{2, 0}, {0, 0}}, 1, 0); // 删除后内部点恢复为极点。

    for (int tc = 0; tc < 250; ++ tc) {
        dyn.init(1 + rng() % 17);
        V < Point > all;
        V < bool > alive;
        for (int step = 0; step < 240; ++ step) {
            if (all.empty() || rng() % 3) {
                Point q{(int)(rng() % 21) - 10, (int)(rng() % 21) - 10};
                assert(dyn.add(q) == (int)all.size());
                all.push_back(q); alive.push_back(true);
            } else {
                int id = (int)(rng() % (all.size() + 2)) - 1;
                bool valid = id >= 0 && id < (int)all.size() && alive[id];
                assert(dyn.erase(id) == valid);
                if (valid) alive[id] = false;
            }
            V < Point > active;
            for (int i = 0; i < (int)all.size(); ++ i)
                if (alive[i]) active.push_back(all[i]);
            for (int j = 0; j < 5; ++ j)
                check_query(dyn, active, (int)(rng() % 41) - 20,
                            (int)(rng() % 41) - 20);
            if (step % 60 == 0) {
                auto now = dyn.snapshot(); check_hull(now, active);
                Point q{(int)(rng() % 31) - 15, (int)(rng() % 31) - 15};
                assert(dyn.contains(q) == brute_contains(active, q));
            }
        }
        for (int i = 0; i < (int)all.size(); ++ i)
            if (alive[i]) assert(dyn.erase(i));
        check_query(dyn, {}, 1, -1); assert(dyn.snapshot().h.empty());
    }
    // 大凸链使每块保留全部极点，避免仅测内部点很多的随机云。
    dyn.init(128);
    V < int > ids;
    for (ll x = -5000; x <= 5000; ++ x) ids.push_back(dyn.add({x, x * x}));
    ll ans;
    assert(dyn.query(0, -1, ans) && ans == 0);
    assert(dyn.erase(ids[5000]));
    assert(dyn.query(0, -1, ans) && ans == -1);
    assert(dyn.query(0, 1, ans) && ans == 25000000);
    cout << "hull_query_random OK: exhaustive subsets, random static / dynamic, seed "
         << seed << '\n';
}
