#include "../../template/geometry/三维凸包.cpp"
#include <bits/stdc++.h>

using ll = long long;
using A3 = array < ll, 3 >;

A3 sub(A3 a, A3 b) {
    return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}
A3 det(A3 a, A3 b) {
    return {a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]};
}
ll dot(A3 a, A3 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// 独立整数暴力：枚举支撑平面，平面内做二维凸包，逐面计算面积体积。
pair < db, db > brute(V < A3 > p) {
    int n = p.size();
    set < array < ll, 4 > > planes;
    for (int i = 0; i < n; ++ i)
        for (int j = i + 1; j < n; ++ j)
            for (int k = j + 1; k < n; ++ k) {
                A3 w = det(sub(p[j], p[i]), sub(p[k], p[i]));
                ll g = gcd(gcd(abs(w[0]), abs(w[1])), abs(w[2]));
                if (! g) continue;
                for (ll &x : w) x /= g;
                ll t = dot(w, p[i]);
                bool pos = 0, neg = 0;
                for (A3 q : p) {
                    pos |= dot(w, q) > t; neg |= dot(w, q) < t;
                }
                if (pos && neg) continue;
                if (! pos && ! neg) continue;
                if (pos) {
                    for (ll &x : w) x = -x;
                    t = -t;
                }
                planes.insert({w[0], w[1], w[2], t});
            }
    db area = 0, volume = 0;
    for (auto plane : planes) {
        A3 w{plane[0], plane[1], plane[2]};
        int drop = max_element(w.begin(), w.end(), [](ll a, ll b) {
            return abs(a) < abs(b);
        }) - w.begin();
        int x = (drop + 1) % 3, y = (drop + 2) % 3;
        V < A3 > q;
        for (A3 a : p) if (dot(w, a) == plane[3]) q.push_back(a);
        sort(q.begin(), q.end(), [&](A3 a, A3 b) {
            return pair{a[x], a[y]} < pair{b[x], b[y]};
        });
        q.erase(unique(q.begin(), q.end()), q.end());
        V < A3 > h;
        auto turn = [&](A3 a, A3 b, A3 c) {
            return (b[x] - a[x]) * (c[y] - a[y]) -
                   (b[y] - a[y]) * (c[x] - a[x]);
        };
        for (A3 a : q) {
            while (h.size() > 1 &&
                   turn(h[h.size() - 2], h.back(), a) <= 0) h.pop_back();
            h.push_back(a);
        }
        int t = h.size();
        for (int i = (int)q.size() - 2; i >= 0; -- i) {
            while ((int)h.size() > t &&
                   turn(h[h.size() - 2], h.back(), q[i]) <= 0) h.pop_back();
            h.push_back(q[i]);
        }
        h.pop_back();
        for (int i = 1; i + 1 < (int)h.size(); ++ i) {
            A3 z = det(sub(h[i], h[0]), sub(h[i + 1], h[0]));
            if (dot(z, w) < 0) for (ll &a : z) a = -a;
            area += sqrtl((db)dot(z, z)) / 2;
            volume += (db)dot(z, h[0]) / 6;
        }
    }
    return {area, fabsl(volume)};
}

void close(db a, db b) {
    if (fabsl(a - b) > 1e-8L * max(1.L, fabsl(b))) {
        cerr << setprecision(18) << a << " != " << b << '\n'; abort();
    }
}

int main() {
    Hull3D hull;
    hull.build({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
    assert(hull.dim == 3 && hull.faces.size() == 4);
    close(hull.area(), (3 + sqrtl(3)) / 2); close(hull.volume(), 1.L / 6);
    V < P3 > cube;
    for (int x = 0; x <= 1; ++ x)
        for (int y = 0; y <= 1; ++ y)
            for (int z = 0; z <= 1; ++ z) cube.push_back({(db)x, (db)y, (db)z});
    cube.push_back(cube[0]); cube.push_back({0.5L, 0.5L, 0.5L});
    hull.build(cube); close(hull.area(), 6); close(hull.volume(), 1);
    hull.build({{0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}});
    assert(hull.dim == 2 && hull.faces.empty());
    close(hull.area(), 0); close(hull.volume(), 0);
    hull.build({{1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {1, 2, 3}});
    assert(hull.dim == 1 && hull.faces.empty());
    hull.build({{1, 2, 3}, {1, 2, 3}}); assert(hull.dim == 0);
    hull.build({}); assert(hull.dim == -1);
    mt19937 rng(712310);
    for (int tc = 0; tc < 1500; ++ tc) {
        V < A3 > p;
        int n = 4 + rng() % 13;
        for (int i = 0; i < n; ++ i)
            p.push_back({(int)(rng() % 9) - 4, (int)(rng() % 9) - 4,
                         (int)(rng() % 9) - 4});
        auto [a, v] = brute(p);
        V < P3 > ps;
        for (A3 q : p) ps.push_back({(db)q[0], (db)q[1], (db)q[2]});
        for (int rep = 0; rep < 3; ++ rep) {
            shuffle(ps.begin(), ps.end(), rng); hull.build(ps);
            close(hull.area(), a); close(hull.volume(), v);
            map < pair < int, int >, int > edges;
            for (auto f : hull.faces) {
                P3 w = hull.normal(f); assert(w.abs() > 0);
                for (P3 q : ps)
                    assert(w.dot(q - ps[f.a]) <= 1e-8L * w.abs());
                edges[{f.a, f.b}] ++; edges[{f.b, f.c}] ++;
                edges[{f.c, f.a}] ++;
            }
            for (auto [edge, cnt] : edges) {
                assert(cnt == 1);
                assert(edges.count({edge.second, edge.first}));
            }
        }
    }
    cout << "hull3d_random OK: 1500 integer sets, 3 orders each\n";
}
