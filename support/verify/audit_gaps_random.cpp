#include <cassert>
#include "../../template/math/Matrix.cpp"
#include "../../template/math/AdaptiveSimpson.cpp"
#include "../../template/math/Combination.cpp"
#include "../../template/math/PrimeSieve.cpp"
#include "../../template/math/LinearSieve.cpp"
#include "../../template/math/Lucas.cpp"
#include "../../template/math/QuotientBlocks.cpp"
#include "../../template/others/Common.cpp"
#include "../../template/others/TernarySearch.cpp"
#include "../../template/graph/TopologicalSort.cpp"
#include "../../template/graph/VBCC.cpp"
#include "../../template/geometry/Point.cpp"
#include "../../template/geometry/Line.cpp"
#include "../../template/geometry/Polygon.cpp"
#include "../../template/string/Hash.cpp"

// 本次审计补足固定样例、同算法真值和点双最大性方面的缺口。
Matrix direct_mul(const Matrix &a, const Matrix &b, ll mod) {
    Matrix c(a.size(), V < ll > (b[0].size()));
    for (int i = 0; i < int(a.size()); ++ i)
        for (int j = 0; j < int(b[0].size()); ++ j) {
            __int128_t sum = 0;
            for (int k = 0; k < int(b.size()); ++ k) sum += __int128_t(a[i][k]) * b[k][j];
            c[i][j] = ll(sum % mod);
        }
    return c;
}

void check_math(mt19937_64 &rng) {
    const ll mods[] = {2, 17, 998244353, 2147483648LL};
    const int ps[] = {2, 3, 5, 7, 17, 101, 997};
    Comb comb; Lucas lucas; Sieve sieve;
    for (int rep = 0; rep < 1000; ++ rep) {
        int n = 1 + rng() % 5, m = 1 + rng() % 5, p = 1 + rng() % 5;
        ll mod = mods[rep % 4];
        Matrix a(n, V < ll > (m)), b(m, V < ll > (p));
        for (auto &row : a) for (ll &x : row) x = rng() % mod;
        for (auto &row : b) for (ll &x : row) x = rng() % mod;
        assert(matrix_mul(a, b, mod) == direct_mul(a, b, mod));
        a.assign(n, V < ll > (n));
        for (auto &row : a) for (ll &x : row) x = rep % 10 == 0 ? mod - 1 : rng() % mod;
        Matrix want(n, V < ll > (n));
        for (int i = 0; i < n; ++ i) want[i][i] = 1;
        int e = rng() % 16;
        for (int i = 0; i < e; ++ i) want = direct_mul(want, a, mod);
        assert(matrix_pow(a, e, mod) == want);
        Matrix id(n, V < ll > (n));
        for (int i = 0; i < n; ++ i) id[i][i] = 1;
        assert(matrix_pow(id, LLONG_MAX, mod) == id);

        mod = ps[rng() % 7]; n = rng() % min(100LL, mod);
        comb.init(n, mod); lucas.init(int(mod));
        V < V < ll > > c(201, V < ll > (201));
        c[0][0] = 1;
        for (int i = 1; i <= 200; ++ i) {
            c[i][0] = 1;
            for (int j = 1; j <= i; ++ j) c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % mod;
        }
        for (int q = 0; q < 30; ++ q) {
            int x = rng() % (n + 1), y = rng() % (x + 1);
            assert(comb.choose(x, y) == c[x][y]);
            x = rng() % 201; y = rng() % (x + 1);
            assert(lucas.choose(x, y) == c[x][y]);
        }
        assert(comb.choose(n + 1, 0) == 0 && comb.choose(n, -1) == 0);
        n = rng() % 301;
        sieve.build(n);
        V < int > prime;
        for (int x = 1; x <= n; ++ x) {
            int lp = x, phi = 0, mu = 1, value = x;
            for (int d = 2; d <= x; ++ d) if (x % d == 0) { lp = d; break; }
            for (int y = 1; y <= x; ++ y) phi += gcd(x, y) == 1;
            for (int d = 2; d <= value; ++ d) if (value % d == 0) {
                int cnt = 0;
                while (value % d == 0) value /= d, ++ cnt;
                mu = cnt > 1 ? 0 : -mu;
            }
            if (x >= 2 && lp == x) prime.push_back(x);
            if (x >= 2) assert(sieve.lp[x] == lp);
            assert(sieve.phi[x] == phi && sieve.mu[x] == mu);
        }
        assert(sieve.prime == prime && primes(n) == prime);
        n = rng() % 10001;
        ll next = 1;
        for (auto [l, r] : div_blocks(n)) {
            assert(l == next && l <= r && r <= n);
            for (ll x = l; x <= r; ++ x) assert(n / x == n / l);
            assert(r == n || n / (r + 1) != n / l);
            next = r + 1;
        }
        assert(next == n + 1);
        mod = 1 + rng() % 1000000000;
        ll x = rng() % mod, y = rng() % mod, z = x;
        add(z, y, mod); assert(z == (x + y) % mod);
        sub(z, y, mod); assert(z == x);
        z = x; bool changed = chkmin(z, y);
        assert(z == min(x, y) && changed == (y < x));
        z = x; changed = chkmax(z, y);
        assert(z == max(x, y) && changed == (y > x));
    }
    cout << "audit math: 1000 rounds, independent matrix / Pascal / trial division / division blocks\n";
}

bool check_numeric(mt19937_64 &rng) {
    bool ok = true;
    for (int rep = 0; rep < 2000; ++ rep) {
        double l = (int(rng() % 61) - 30) / 10., r = l + (rng() % 31) / 10.;
        V < double > c(1 + rng() % 7);
        for (double &x : c) x = (int(rng() % 21) - 10) / 10.;
        auto f = [&](double x) {
            double y = 0;
            for (int i = int(c.size()) - 1; i >= 0; -- i) y = y * x + c[i];
            return y;
        };
        long double want = 0;
        for (int i = 0; i < int(c.size()); ++ i)
            want += c[i] * (powl(r, i + 1) - powl(l, i + 1)) / (i + 1);
        double actual = simpson(f, l, r, 1e-10);
        if (abs(actual - want) >= 1e-7L * max(1.L, abs(want))) {
            ok = false;
            cerr << setprecision(20) << "Simpson counterexample rep=" << rep
                 << " l=" << l << " r=" << r << " actual=" << actual << " expected=" << want << '\n';
            cerr << "coefficients (ascending):";
            for (double x : c) cerr << ' ' << x;
            cerr << '\n';
        }
        assert(simpson(f, l, l) == 0);
        ld center = (int(rng() % 101) - 50) / 10.L;
        ld a = (int(rng() % 101) - 50) / 10.L, b = a + (rng() % 51) / 10.L;
        ld k = 1 + rng() % 10, expected = clamp(center, a, b);
        auto convex = [&](ld x) { return k * (x - center) * (x - center); };
        auto concave = [&](ld x) { return -convex(x); };
        assert(abs(ternary_min(convex, a, b) - expected) < 1e-7L);
        assert(abs(ternary_max(concave, a, b) - expected) < 1e-7L);
    }
    assert(abs(simpson([](double x) { return sin(x); }, 0, acos(-1.)) - 2) < 1e-7);
    assert(ternary_min([](ld) { return 1.L; }, 3, 3) == 3);
    auto alias = [](double x) { return x * x * x * x * (0.2 - x * x); };
    double actual = simpson(alias, -0.1, 0.3, 1e-10);
    long double expected = 1161.L / 17500000;
    if (abs(actual - expected) > 1e-10L) {
        cerr << setprecision(20) << "Simpson fixed alias: actual=" << actual
             << " expected=" << expected << " error=" << abs(actual - expected) << '\n';
        ok = false;
    }
    // 保留原误差阈值；平移、缩放并叠加任意三次项，防止仅绕过一个固定反例。
    for (int rep = 0; rep < 1000; ++ rep) {
        double shift = (int(rng() % 65) - 32) / 4.;
        double stretch = (1 + rng() % 16) / 4.;
        double scale = int(rng() % 65) - 32;
        V < double > c(4);
        for (double &x : c) x = (int(rng() % 41) - 20) / 10.;
        auto f = [&](double x) {
            double t = (x - shift) / stretch;
            return ((c[3] * t + c[2]) * t + c[1]) * t + c[0] +
                   scale * t * t * t * t * (0.2 - t * t);
        };
        long double want = scale * (1161.L / 17500000);
        for (int i = 0; i < 4; ++ i)
            want += c[i] * (powl(0.3L, i + 1) - powl(-0.1L, i + 1)) / (i + 1);
        want *= stretch;
        double l = shift - stretch / 10, r = shift + stretch * 3 / 10;
        assert(abs(simpson(f, l, r, 1e-10) - want) <= 1e-10L);
        assert(abs(simpson(f, r, l, 1e-10) + want) <= 1e-10L);
    }
    // 深度为 0 时保持有界退化行为，不宣称达到 eps。
    assert(isfinite(simpson(alias, -0.1, 0.3, 1e-10, 0)));
    assert(abs(simpson([](double x) { return exp(x); }, 0, 1, 1e-10) - expm1(1.)) < 1e-10);
    assert(abs(simpson([](double x) { return 1 / (1 + x * x); }, -1, 1, 1e-10) - acos(-1.) / 2) < 1e-10);
    // 有限采样值与有限积分仍可能因中点或加权和溢出；真值用解析对数。
    for (int rep = 0; rep < 1000; ++ rep) {
        double l = pow(10., int(rng() % 617) - 308);
        double r = l * (1 + (1 + rng() % 10) / 100.);
        auto reciprocal = [](double x) { return 1 / x; };
        long double want = log(static_cast < long double > (r) / l);
        double actual = simpson(reciprocal, l, r, 1e-10);
        assert(isfinite(actual) && abs(actual - want) <= 1e-10L);
        actual = simpson(reciprocal, r, l, 1e-10);
        assert(isfinite(actual) && abs(actual + want) <= 1e-10L);
    }
    for (double l : {1e-308, 1e308}) {
        double r = 1.1 * l;
        long double want = log(static_cast < long double > (r) / l);
        double actual = simpson([](double x) { return 1 / x; }, l, r, 1e-10);
        assert(isfinite(actual) && abs(actual - want) <= 1e-10L);
    }
    cout << "audit numeric: 2000 analytic polynomials and clipped quadratic extrema\n";
    cout << "audit Simpson: fixed alias, 1000 affine / cubic variants in both directions, exp / atan\n";
    cout << "audit Simpson: 1000 reciprocal intervals across 1e-308 to 1e308, both directions\n";
    return ok;
}

void check_graph(mt19937_64 &rng) {
    for (int rep = 0; rep < 1500; ++ rep) {
        int n = rng() % 8;
        V < V < int > > g(n + 1), reach(n + 1, V < int > (n + 1));
        VBCC vbcc(n);
        for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v)
            if (rng() % 5 == 0) {
                g[u].push_back(v); reach[u][v] = 1;
                if (rng() % 2) g[u].push_back(v);
                if (u != v) vbcc.add_edge(u, v);
            }
        for (int k = 1; k <= n; ++ k) for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= n; ++ v) reach[u][v] |= reach[u][k] && reach[k][v];
        bool acyclic = true;
        for (int u = 1; u <= n; ++ u) if (reach[u][u]) acyclic = false;
        V < int > order; assert(topo(g, order) == acyclic);
        if (acyclic) {
            V < int > pos(n + 1, -1);
            assert(int(order.size()) == n);
            for (int i = 0; i < n; ++ i) {
                int u = order[i]; assert(u >= 1 && u <= n && pos[u] == -1); pos[u] = i;
            }
            for (int u = 1; u <= n; ++ u) for (int v : g[u]) assert(pos[u] < pos[v]);
        }
        // 枚举所有点集，独立检查连通且删任一点后仍连通，再取极大集合。
        auto connected = [&](int mask) {
            if (! mask) return true;
            int start = __builtin_ctz(unsigned(mask)) + 1, seen = 0;
            auto dfs = [&](auto &&self, int u) -> void {
                seen |= 1 << (u - 1);
                for (auto [v, id] : vbcc.g[u]) {
                    (void)id;
                    if ((mask >> (v - 1) & 1) && ! (seen >> (v - 1) & 1)) self(self, v);
                }
            };
            dfs(dfs, start); return seen == mask;
        };
        V < int > candidates;
        for (int mask = 1; mask < (1 << n); ++ mask) {
            bool ok = connected(mask);
            for (int u = 1; u <= n && ok; ++ u)
                if (mask >> (u - 1) & 1) ok = connected(mask ^ (1 << (u - 1)));
            if (ok) candidates.push_back(mask);
        }
        set < int > want;
        for (int mask : candidates) {
            bool maximal = true;
            for (int other : candidates) if (other != mask && (other & mask) == mask) maximal = false;
            if (maximal) want.insert(mask);
        }
        for (int repeat = 0; repeat < 2; ++ repeat) {
            vbcc.build(); set < int > got;
            for (const auto &block : vbcc.bcc) {
                int mask = 0;
                for (int u : block.v) mask |= 1 << (u - 1);
                assert(got.insert(mask).second);
            }
            assert(got == want);
        }
    }
    cout << "audit graph: 1500 cycle checks and exhaustive maximal vertex blocks\n";
}

ll orientation(P a, P b, P c) {
    ll x1 = ll(b.x - a.x), y1 = ll(b.y - a.y);
    ll x2 = ll(c.x - a.x), y2 = ll(c.y - a.y);
    return x1 * y2 - y1 * x2;
}

void check_geometry_hash(mt19937_64 &rng) {
    auto point = [&]() { return P(int(rng() % 41) - 20, int(rng() % 41) - 20); };
    auto sg = [](ll x) { return (x > 0) - (x < 0); };
    for (int rep = 0; rep < 5000; ++ rep) {
        P a = point(), b = point(), c = point(), d = point();
        ll ab = orientation(a, b, c), ad = orientation(a, b, d);
        ll ca = orientation(c, d, a), cb = orientation(c, d, b);
        assert(cross(a, b, c) == ab);
        assert(a.dot(b) == ll(a.x) * ll(b.x) + ll(a.y) * ll(b.y));
        assert((a + b).x == a.x + b.x && (a - b).y == a.y - b.y);
        assert(abs(a.rot90().dot(a)) < 1e-12L);
        assert(abs(a.distTo(b) - hypotl(a.x - b.x, a.y - b.y)) < 1e-12L);
        bool overlap = max(min(a.x, b.x), min(c.x, d.x)) <= min(max(a.x, b.x), max(c.x, d.x)) &&
                       max(min(a.y, b.y), min(c.y, d.y)) <= min(max(a.y, b.y), max(c.y, d.y));
        bool touch = overlap && sg(ab) * sg(ad) <= 0 && sg(ca) * sg(cb) <= 0;
        assert(isSS(a, b, c, d) == touch);
        assert(isSS_strict(a, b, c, d) == (sg(ab) * sg(ad) < 0 && sg(ca) * sg(cb) < 0));
        if (! (a == b)) {
            P h = proj(a, b, c), reflected = reflect(a, b, c);
            assert(abs((h - c).dot(b - a)) < 1e-8L);
            assert(abs((h - a).det(b - a)) < 1e-8L);
            assert((reflected + c).distTo(h * 2) < 1e-8L);
            ld t = ((c.x - a.x) * (b.x - a.x) + (c.y - a.y) * (b.y - a.y)) /
                   ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
            t = clamp(t, 0.L, 1.L);
            assert(abs(nearest(a, b, c) - hypotl(c.x - a.x - t * (b.x - a.x),
                                               c.y - a.y - t * (b.y - a.y))) < 1e-8L);
            if (! (c == d) && orientation(P(0, 0), b - a, d - c) != 0) {
                P p = isLL(a, b, c, d);
                assert(abs((p - a).det(b - a)) < 1e-8L && abs((p - c).det(d - c)) < 1e-8L);
            }
        }
        ld l = int(rng() % 21) - 10, r = l + 1 + rng() % 10;
        ld low = int(rng() % 21) - 10, high = low + 1 + rng() % 10;
        V < P > rect{{l, low}, {r, low}, {r, high}, {l, high}};
        assert(area(rect) == (r - l) * (high - low));
        bool inside = l < c.x && c.x < r && low < c.y && c.y < high;
        bool boundary = l <= c.x && c.x <= r && low <= c.y && c.y <= high && ! inside;
        assert(contain(rect, c) == (inside ? 2 : boundary ? 1 : 0));
        ld cut = int(rng() % 41) - 20;
        assert(abs(area(convexCut(rect, {cut, low}, {cut, high})) -
                   (clamp(cut, l, r) - l) * (high - low)) < 1e-8L);
        reverse(rect.begin(), rect.end());
        assert(contain(rect, c) == (inside ? 2 : boundary ? 1 : 0));
        string s(rng() % 31, '\0');
        for (char &x : s) x = char(rng() % 256);
        Hash h(s);
        if (! s.empty()) {
            int x = rng() % s.size(), y = rng() % s.size(); if (x > y) swap(x, y);
            ll h1 = 0, h2 = 0;
            for (int i = x; i <= y; ++ i) {
                h1 = (h1 * Hash::base + (unsigned char)s[i] + 1) % Hash::mod1;
                h2 = (h2 * Hash::base + (unsigned char)s[i] + 1) % Hash::mod2;
            }
            assert(h.query(x, y) == make_pair(h1, h2));
        }
    }
    assert(isSS({0, 0}, {0, 0}, {0, 0}, {1, 0}));
    assert(! isSS_strict({0, 0}, {1, 0}, {1, 0}, {2, 0}));
    assert(nearest({0, 0}, {0, 0}, {3, 4}) == 5);
    assert(contain({}, {0, 0}) == 0 && convexCut({}, {0, 0}, {1, 0}).empty());
    cout << "audit geometry/hash: 5000 integer predicates, rectangle cuts and direct byte hashes\n";
}

int main() {
    mt19937_64 rng(2026101201);
    check_math(rng);
    bool numeric_ok = check_numeric(rng);
    check_graph(rng); check_geometry_hash(rng);
    cout << flush;
    assert(numeric_ok);
    cout << "PASS: audit gaps, random truth and special cases\n";
}
