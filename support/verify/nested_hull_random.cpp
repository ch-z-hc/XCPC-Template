#define INSIDE_TRIANGLE_NO_MAIN
#include "inside_triangle_submission.cpp"

using Point = NestedHull::P;
using Wide = __int128_t;

// 真值使用宽整数，枚举全部内点、全部弦与全部三角形。
Wide cross(Point a, Point b, Point c) {
    return Wide(b.x - a.x) * (c.y - a.y) - Wide(b.y - a.y) * (c.x - a.x);
}

V < Point > convex(V < Point > p) {
    sort(p.begin(), p.end(), [](Point a, Point b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });
    p.erase(unique(p.begin(), p.end(), [](Point a, Point b) {
        return a.x == b.x && a.y == b.y;
    }), p.end());
    V < Point > h;
    for (Point a : p) {
        while (h.size() > 1 && cross(h[h.size() - 2], h.back(), a) <= 0) h.pop_back();
        h.push_back(a);
    }
    int k = h.size();
    for (int i = (int)p.size() - 2; i >= 0; -- i) {
        while ((int)h.size() > k && cross(h[h.size() - 2], h.back(), p[i]) <= 0) h.pop_back();
        h.push_back(p[i]);
    }
    h.pop_back();
    return h;
}

bool inside(const V < Point > & p, const V < Point > & q) {
    for (int i = 0; i < (int)p.size(); ++ i)
        for (Point x : q)
            if (cross(p[i], p[(i + 1) % p.size()], x) <= 0) return 0;
    return 1;
}

ll check(const V < Point > & p, const V < Point > & q) {
    static NestedHull h; // 复用对象，覆盖大小变化时的状态重置。
    h.build(p, q);
    int n = p.size(), m = q.size();
    assert(inside(p, q));
    V < V < bool > > good(n, V < bool > (n));
    for (int i = 0; i < n; ++ i) {
        assert(0 <= h.left[i] && h.left[i] < m);
        assert(0 <= h.right[i] && h.right[i] < m);
        for (Point x : q) {
            assert(cross(p[i], q[h.left[i]], x) >= 0);
            assert(cross(p[i], q[h.right[i]], x) <= 0);
        }
        assert(cross(p[i], q[h.left[i]], q[(h.left[i] + 1) % m]) > 0);
        assert(cross(p[i], q[h.right[i]], q[(h.right[i] + 1) % m]) < 0);
        int r = i, l = i + n;
        for (int j = i + 1; j < i + n; ++ j) {
            bool a = 1, b = 1;
            for (Point x : q) {
                Wide d = cross(p[i], p[j % n], x);
                a = a && d >= 0; b = b && d <= 0;
            }
            good[i][j % n] = a;
            if (a) r = j;
            if (b) l = min(l, j);
            assert(a == (j <= h.r[i]));
            assert(b == (j >= h.l[i]));
        }
        assert(h.r[i] == r && h.l[i] == l);
        assert(i < r && r < l && l < i + n);
        assert(h.r[i + n] == r + n && h.l[i + n] == l + n);
    }
    assert(is_sorted(h.r.begin(), h.r.end()));
    assert(is_sorted(h.l.begin(), h.l.end()));
    ll ans = 0;
    for (int i = 0; i < n; ++ i)
        for (int j = i + 1; j < n; ++ j)
            for (int k = j + 1; k < n; ++ k)
                ans += good[i][j] && good[j][k] && good[k][i];
    assert(count_triangles(p, q) == ans);
    return ans;
}

int main() {
    V < Point > p = {{-5, 0}, {-4, -4}, {0, -4}, {3, 1}, {2, 5}, {-3, 4}};
    assert(check(p, {{-2, 1}, {-2, 0}, {0, 0}, {0, 1}}) == 2);
    assert(check(p, {{-1, 1}, {-1, 0}, {0, 0}, {0, 1}}) == 4);
    assert(check({{1, -3}, {4, -1}, {5, 0}, {1, 4}, {0, 4}, {-1, 0}, {0, -2}},
                 {{0, 0}, {2, 0}, {0, 2}}) == 6);
    assert(check({{0, 0}, {20, 0}, {0, 20}}, {{1, 1}, {3, 1}, {1, 3}}) == 1);
    // 外顶点与内边共线；旋转双方起点覆盖切线平台及跨圈。
    p = {{-4, -4}, {4, -4}, {6, 1}, {4, 4}, {-4, 4}, {-6, 1}};
    V < Point > q = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
    for (int i = 0; i < 6; ++ i) {
        for (int j = 0; j < 4; ++ j) {
            check(p, q);
            rotate(q.begin(), q.begin() + 1, q.end());
        }
        rotate(p.begin(), p.begin() + 1, p.end());
    }
    // 叉积接近 ll 上限；极细内多边形及最小整数面积。
    check({{-1000000000, -1000000000}, {1000000000, -1000000000},
           {1000000000, 1000000000}, {-1000000000, 1000000000}},
          {{-999999999, -999999999}, {999999999, -999999999},
           {999999999, 999999999}, {-999999999, 999999999}});
    check({{-1000000000, -2}, {1000000000, -2}, {1000000000, 2}, {-1000000000, 2}},
          {{0, 0}, {1, 0}, {0, 1}});
    mt19937 rng(14714);
    for (int t = 0; t < 10000; ++ t) {
        p.clear(); q.clear();
        for (int i = 0; i < 5 + t % 25; ++ i)
            p.push_back({int(rng() % 201) - 100, int(rng() % 201) - 100});
        for (int i = 0; i < 3 + t % 15; ++ i)
            q.push_back({int(rng() % 101) - 50, int(rng() % 101) - 50});
        p = convex(p); q = convex(q);
        if (p.size() < 3 || q.size() < 3 || ! inside(p, q)) { -- t; continue; }
        rotate(p.begin(), p.begin() + rng() % p.size(), p.end());
        rotate(q.begin(), q.begin() + rng() % q.size(), q.end());
        check(p, q);
        if (t % 10 == 0) {
            // 正行列式整系数变换与大平移，不改变拓扑或相切关系。
            for (auto * a : {&p, &q}) for (Point & x : *a)
                x = {1000000 * (2 * x.x + x.y) + 700000000,
                     1000000 * (x.x + x.y) - 700000000};
            check(p, q);
        }
    }
    // 大规模严格凸链，验证两个内外指针均需扫描的情形。
    p.clear(); q.clear();
    for (ll x = -20000; x <= 20000; ++ x) {
        p.push_back({2 * x, 2 * x * x});
        q.push_back({x, x * x + 200000000});
    }
    NestedHull h; h.build(p, q);
    assert(is_sorted(h.r.begin(), h.r.end()));
    assert(is_sorted(h.l.begin(), h.l.end()));
    for (int i = 0; i < (int)p.size(); i += 97) {
        for (Point x : q) {
            assert(cross(p[i], q[h.left[i]], x) >= 0);
            assert(cross(p[i], q[h.right[i]], x) <= 0);
            assert(cross(p[i], p[h.r[i] % p.size()], x) >= 0);
        }
    }
    cout << "PASS: nested tangents, chord ranges, samples, 10000 random cases and large hulls\n";
}
