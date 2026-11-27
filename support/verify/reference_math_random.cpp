#include "../../template/math/多项式基本运算.cpp"
#include "../../template/math/组合数.cpp"
#include "../../template/math/多项式多点求值与快速插值.cpp"
#include "../../template/math/快速傅里叶变换与任意模数卷积.cpp"
#include "../../template/math/快速沃尔什变换.cpp"
#include "../../template/math/杨辉三角.cpp"
#include "../../template/math/多项式微积分与转置卷积.cpp"
#include "../../template/math/多项式平移与等比求值.cpp"
#include "../../template/math/多项式幂投影.cpp"
#include "../../template/math/分块快速阶乘.cpp"
#include "../../template/math/拆系数浮点卷积.cpp"
#include "../../template/math/半在线子集卷积.cpp"
#include "../../template/math/有标号图计数.cpp"

int main() {
    const int mod = FPS::mod;
    mt19937 rng(2711);
    auto value = [&](const V < int > &a, int x) {
        ll ans = 0;
        for (auto it = a.rbegin(); it != a.rend(); it ++) ans = (ans * x + *it) % mod;
        return int(ans);
    };
    for (int test = 0; test < 300; test ++) {
        int n = 1 + rng() % 28, m = rng() % 25;
        V < int > a(n), b(n);
        for (int &x : a) x = rng() % mod;
        for (int i = 1; i < n; i ++) b[i] = rng() % mod;
        V < int > f = a, expect;
        for (int k = 0; k < n; k ++) {
            expect.push_back(f.back());
            V < int > g(n);
            for (int i = 0; i < n; i ++) for (int j = 1; i + j < n; j ++)
                g[i + j] = (g[i + j] + 1ll * f[i] * b[j]) % mod;
            f.swap(g);
        }
        assert(power_projection(a, b, n) == expect);
        int c = test % 5 == 0 ? 0 : test % 5 == 1 ? mod - 2 : rng() % mod;
        auto shift = taylor_shift(a, c), z = chirp_z(a, c, m);
        int p = 1;
        for (int i = 0; i < m; i ++) {
            assert(z[i] == value(a, p));
            p = 1ll * p * c % mod;
        }
        V < int > samples;
        for (int i = 0; i < n; i ++) samples.push_back(value(a, i));
        auto y = shift_samples(samples, c);
        for (int i = 0; i < n; i ++) {
            assert(y[i] == value(a, (c + i) % mod));
            assert(y[i] == value(shift, i));
        }
        assert(poly_deriv(poly_integ(a)) == a);
        auto corr = poly_corr(a, b);
        for (int i = 0; i < n; i ++) {
            ll ans = 0;
            for (int j = 0; j + i < n; j ++) ans = (ans + 1ll * a[i + j] * b[j]) % mod;
            assert(corr[i] == ans);
        }
        auto fold = cyclic_fold(a, 3);
        for (int i = 0; i < 3; i ++) {
            ll ans = 0;
            for (int j = i; j < n; j += 3) ans += a[j];
            assert(fold[i] == ans % mod);
        }
        ull k = (ull(rng()) << 32) | rng();
        a[0] = 1 + rng() % (mod - 1);
        assert(sparse_power(a, k, n) == FPS::pow(a, k, n));
        int pmod = test % 6 == 0 ? 1 : INT_MAX - int(rng() % 5000);
        for (int &x : a) x = int(rng());
        for (int &x : b) x = int(rng());
        auto prod = split_conv(a, b, pmod);
        V < int > check(2 * n - 1);
        for (int i = 0; i < n; i ++) for (int j = 0; j < n; j ++)
            check[i + j] = (check[i + j] + 1ll * a[i] * b[j]) % pmod;
        for (int &x : check) if (x < 0) x += pmod;
        assert(prod == check);
    }
    for (int n : {255, 256, 513}) {
        V < int > a(n);
        for (int &x : a) x = rng() % mod;
        auto f = power_projection(a, {0, 1}, n);
        reverse(a.begin(), a.end()); assert(f == a);
    }
    V < int > dense(4096, INT_MAX - 1);
    auto triangle = split_conv(dense, dense, INT_MAX);
    for (int i = 0; i < int(triangle.size()); i ++)
        assert(triangle[i] == min(i + 1, int(triangle.size()) - i));
    assert(chirp_z({}, 0, 12) == V < int >(12) && shift_samples({}, 0).empty());
    V < int > factorial(4001, 1);
    for (int i = 1; i <= 4000; i ++) factorial[i] = 1ll * i * factorial[i - 1] % mod;
    for (int i = 0; i <= 4000; i += i < 50 ? 1 : 127)
        assert(fast_factorial(i) == factorial[i]);
    assert(fast_factorial(mod) == 0);
    auto c = pascal(50, 1000);
    assert(c[0][0] == 1 && c[50][25] == 752 && c[28][1] == 28);
    for (int n = 0; n <= 8; n ++) {
        int m = 1 << n;
        V < ll > g(m), f(m);
        for (auto &x : g) x = rng() % 99;
        auto update = [](int s, ll x) { return (x + s) % 101; };
        f[0] = 1;
        for (int s = 1; s < m; s ++) {
            ll sum = 0;
            for (int t = s; t; t = (t - 1) & s) sum += g[t] * f[s ^ t];
            f[s] = update(s, sum);
        }
        assert(f == subset_recurrence(g, 101, update));
    }
    GraphCount gc(4);
    auto con = gc.connected(), even = gc.connected(true), bip = gc.bipartite();
    auto dag = gc.dag(), weak = gc.dag(true), strong = gc.strong();
    assert(!con[0] && !even[0] && bip[0] == 1 && dag[0] == 1 && !weak[0] && !strong[0]);
    for (int n = 1; n <= 4; n ++) {
        int cc = 0, ec = 0, bc = 0, dc = 0, wc = 0, sc = 0;
        V < pair < int, int > > und, dir;
        for (int i = 0; i < n; i ++) for (int j = 0; j < n; j ++) {
            if (i != j) dir.emplace_back(i, j);
            if (i < j) und.emplace_back(i, j);
        }
        for (bool directed : {false, true}) {
            auto &edges = directed ? dir : und;
            for (int s = 0; s < (1 << edges.size()); s ++) {
                V < int > reach(n), link(n), deg(n);
                for (size_t i = 0; i < edges.size(); i ++) if ((s >> i) & 1) {
                    auto [a, b] = edges[i];
                    reach[a] |= 1 << b;
                    link[a] |= 1 << b; link[b] |= 1 << a;
                    deg[a] ++; deg[b] ++;
                }
                for (int k = 0; k < n; k ++) for (int i = 0; i < n; i ++) {
                    if ((reach[i] >> k) & 1) reach[i] |= reach[k];
                    if ((link[i] >> k) & 1) link[i] |= link[k];
                }
                bool connected = (link[0] | 1) == (1 << n) - 1;
                if (directed) {
                    bool acyclic = true, all = true;
                    for (int i = 0; i < n; i ++) {
                        acyclic &= !(reach[i] & (1 << i));
                        all &= (reach[i] | (1 << i)) == (1 << n) - 1;
                    }
                    dc += acyclic; wc += acyclic && connected; sc += all;
                } else {
                    cc += connected;
                    ec += connected && none_of(deg.begin(), deg.end(), [](int d) { return d & 1; });
                    for (int col = 0; col < (1 << n); col ++) {
                        bool ok = true;
                        for (size_t i = 0; i < edges.size(); i ++) if ((s >> i) & 1) {
                            auto [a, b] = edges[i]; ok &= ((col >> a) ^ (col >> b)) & 1;
                        }
                        if (ok) { bc ++; break; }
                    }
                }
            }
        }
        assert(cc == con[n] && ec == even[n] && bc == bip[n]);
        assert(dc == dag[n] && wc == weak[n] && sc == strong[n]);
    }
}
