#include "../../template/geometry/三维凸包.cpp"
#include "../../template/geometry/三维凸包最大点积查询.cpp"

mt19937 rng(20261110);

void check(HullQuery3D &hull, const V < P3 > &p, P3 q) {
    db ans = 1234567;
    bool ok = hull.query(q.x, q.y, q.z, ans);
    assert(ok == ! p.empty());
    if (! ok) {
        assert(ans == 1234567 && hull.pos == -1);
        return;
    }
    // 独立枚举所有原点，不使用凸包或其点积函数作为答案。
    db want = -numeric_limits < db >::infinity();
    for (P3 t : p) want = max(want, t.x * q.x + t.y * q.y + t.z * q.z);
    if (fabsl(ans - want) > 1e-11L * max(1.L, fabsl(want))) {
        cerr << setprecision(20) << "query " << q.x << ' ' << q.y << ' ' << q.z
             << " got " << ans << " want " << want << " pos " << hull.pos << '\n';
        for (P3 t : p) cerr << t.x << ' ' << t.y << ' ' << t.z << '\n';
        abort();
    }
    assert(hull.pos >= 0 && hull.pos < (int)p.size());
    P3 t = p[hull.pos];
    assert(ans == t.x * q.x + t.y * q.y + t.z * q.z);
}

void test(const V < P3 > &p, bool all_starts = 0) {
    HullQuery3D hull; hull.build(p);
    for (int a = -2; a <= 2; ++ a)
        for (int b = -2; b <= 2; ++ b)
            for (int c = -2; c <= 2; ++ c) {
                P3 q{(db)a, (db)b, (db)c};
                check(hull, p, q);
                if (all_starts && hull.hull.dim == 3)
                    for (int i = 0; i < (int)p.size(); ++ i)
                        if (! hull.g[i].empty()) hull.pos = i, check(hull, p, q);
            }
    for (int i = 0; i < 150; ++ i) {
        P3 q{(db)((int)(rng() % 201) - 100),
             (db)((int)(rng() % 201) - 100),
             (db)((int)(rng() % 201) - 100)};
        check(hull, p, q); check(hull, p, q * -1);
        check(hull, p, q * -1); // 重复方向复用答案。
    }
    hull.build({}); check(hull, {}, {1, 2, 3});
    hull.build({{7, -3, 2}}); check(hull, {{7, -3, 2}}, {-2, 1, -1});
    hull.build(p); check(hull, p, {0, 0, 0});
}

int main() {
    V < P3 > cube;
    for (int x : {-1, 1}) for (int y : {-1, 1}) for (int z : {-1, 1})
        cube.push_back({(db)x, (db)y, (db)z});
    for (int mask = 0; mask < 256; ++ mask) {
        V < P3 > p;
        for (int i = 0; i < 8; ++ i) if (mask >> i & 1) p.push_back(cube[i]);
        test(p, 1);
    }
    test(V < P3 >(20, {3, -2, 7}));
    V < P3 > line, plane, grid;
    for (int x = -3; x <= 3; ++ x) {
        line.push_back({(db)x, (db)(2 * x + 1), (db)(-3 * x + 2)});
        for (int y = -3; y <= 3; ++ y) {
            plane.push_back({(db)x, (db)y, (db)(2 * x - y + 5)});
            for (int z = -3; z <= 3; ++ z)
                grid.push_back({(db)x, (db)y, (db)z});
        }
    }
    test(line); test(plane); test(grid, 1);
    for (int t = 0; t < 2000; ++ t) {
        V < P3 > p;
        int n = rng() % 41;
        for (int i = 0; i < n; ++ i) {
            db x = (int)(rng() % 21) - 10, y = (int)(rng() % 21) - 10;
            db z = (int)(rng() % 21) - 10;
            if (t % 5 == 0) z = 2 * x - 3 * y + 4;
            if (t % 7 == 0) y = x * 2, z = 3 - x;
            p.push_back({x, y, z});
            if (rng() % 5 == 0) p.push_back(p.back());
        }
        shuffle(p.begin(), p.end(), rng);
        test(p);
    }
    V < P3 > sphere;
    for (int i = 0; i < 180; ++ i) {
        db z = 1 - 2 * (i + 0.5L) / 180, a = i * 2.399963229728653L;
        db r = sqrtl(1 - z * z);
        sphere.push_back({r * cosl(a), r * sinl(a), z});
    }
    test(sphere, 1);
    for (P3 &p : sphere) p = p * 1000 + P3{500, -700, 300};
    test(sphere);
    puts("PASS: 3D support query exhaustive subsets, 2000 random sets, degeneracy, ties, rebuild and sphere");
}
