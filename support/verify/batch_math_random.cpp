#include "../../template/math/积性函数求和.cpp"
#include "../../template/math/单纯形与线性规划.cpp"
#include "../../template/math/高斯消元.cpp"

#include <bits/stdc++.h>

ll power(ll a, int e, ll mod) {
    ll ans = 1;
    for (; e; e >>= 1, a = a * a % mod) if (e & 1) ans = ans * a % mod;
    return ans;
}

void check_min25() {
    mt19937 rng(712369);
    Min25 sieve;
    for (int rep = 0; rep < 1600; ++ rep) {
        int n = int(rng() % 1000);
        ll mod = rep % 3 == 0 ? INT_MAX : 2 + rng() % 100000;
        array < ll, 3 > c;
        for (ll &x : c) x = int(rng() % 11) - 5;
        auto f = [&](ll p, int e) {
            ll x = power(p % mod, e, mod);
            return sieve.norm((c[0] * e + c[1] * x +
                               c[2] * (x * x % mod)) % mod);
        };
        sieve.init(n, mod);
        ll ans = 0, primes = 0;
        for (int x = 1; x <= n; ++ x) {
            int y = x, factors = 0;
            ll val = 1;
            for (int p = 2; p <= y / p; ++ p) if (y % p == 0) {
                int e = 0;
                while (y % p == 0) y /= p, ++ e;
                val = val * f(p, e) % mod;
                ++ factors;
            }
            if (y > 1) val = val * f(y, 1) % mod;
            if (x > 1 && factors == 0) ++ primes;
            ans = (ans + val) % mod;
        }
        assert(sieve.pi() == primes);
        assert(sieve.sum(c, f) == ans);
        assert(sieve.sum(c, f) == ans);
        auto one = [](ll, int) { return 1; };
        assert(sieve.sum({1, 0, 0}, one) == n % mod);
    }
    sieve.init(1000000, 1000000007);
    assert(sieve.pi() == 78498);
    auto phi = [](ll p, int e) {
        ll ans = p - 1;
        while (-- e) ans *= p;
        return ans % 1000000007;
    };
    V < int > phi_table(1000001);
    iota(phi_table.begin(), phi_table.end(), 0);
    for (int p = 2; p <= 1000000; ++ p) if (phi_table[p] == p)
        for (int x = p; x <= 1000000; x += p) phi_table[x] -= phi_table[x] / p;
    ll sum = 0;
    for (int i = 1; i <= 1000000; ++ i) sum = (sum + phi_table[i]) % 1000000007;
    assert(sieve.sum({-1, 1, 0}, phi) == sum);
    // 原始平方和已超出 ll；验证先约分、再取模对合数同样成立。
    for (ll mod : {6LL, 2147483647LL}) {
        const ll n = 10000000;
        sieve.init(n, mod);
        auto square = [&](ll p, int e) {
            ll x = power(p % mod, e, mod);
            return x * x % mod;
        };
        __int128_t expected = __int128_t(n) * (n + 1) * (2 * n + 1) / 6 % mod;
        assert(sieve.sum({0, 0, 1}, square) == expected);
    }
}

optional < ld > vertices(V < V < ld > > A, V < ld > b, const V < ld > &c) {
    int n = int(c.size());
    for (int j = 0; j < n; ++ j) {
        V < ld > row(n); row[j] = -1;
        A.push_back(row); b.push_back(0);
    }
    optional < ld > best;
    int m = int(A.size());
    for (int mask = 0; mask < (1 << m); ++ mask) {
        if (__builtin_popcount(unsigned(mask)) != n) continue;
        V < V < ld > > eq;
        for (int i = 0; i < m; ++ i) if (mask >> i & 1) {
            eq.push_back(A[i]); eq.back().push_back(b[i]);
        }
        V < ld > x;
        if (gauss(eq, n, x) != 1) continue;
        bool ok = true;
        for (int i = 0; i < m; ++ i) {
            ld val = inner_product(A[i].begin(), A[i].end(), x.begin(), 0.L);
            if (val > b[i] + 1e-8L) ok = false;
        }
        if (ok) {
            ld val = inner_product(c.begin(), c.end(), x.begin(), 0.L);
            if (! best || val > *best) best = val;
        }
    }
    return best;
}

void check_simplex() {
    Simplex lp;
    assert(lp.solve({}, {}, {1}).status == 2);
    assert(lp.solve({}, {}, {-1}).value == 0);
    assert(lp.solve({}, {}, {}).status == 1);
    assert(lp.solve({{}}, {-1}, {}).status == 0);
    assert(lp.solve({{}}, {1}, {}).status == 1);
    assert(lp.solve({{-1}, {1}}, {-2, 1}, {1}).status == 0);
    // Beale 退化循环例：Bland 规则应终止并得到 1。
    auto beale = lp.solve({{.5L, -5.5L, -2.5L, 9},
                           {.5L, -1.5L, -.5L, 1}, {1, 0, 0, 0}},
                          {0, 0, 1}, {10, -57, -9, -24});
    assert(beale.status == 1 && abs(beale.value - 1) < 1e-8L);
    mt19937 rng(812367);
    for (int rep = 0; rep < 1200; ++ rep) {
        int n = 1 + int(rng() % 4), m = int(rng() % 6);
        V < V < ld > > A(m, V < ld > (n));
        V < ld > b(m), c(n);
        for (auto &row : A) for (ld &x : row) x = int(rng() % 9) - 4;
        for (ld &x : b) x = int(rng() % 17) - 6;
        for (ld &x : c) x = int(rng() % 11) - 5;
        for (int j = 0; j < n; ++ j) {
            V < ld > row(n); row[j] = 1;
            A.push_back(row); b.push_back(1 + rng() % 8);
        }
        auto expected = vertices(A, b, c);
        auto ans = lp.solve(A, b, c);
        assert(ans.status == (expected ? 1 : 0));
        if (! expected) continue;
        assert(abs(ans.value - *expected) < 1e-7L);
        for (ld x : ans.x) assert(x >= -1e-8L);
        for (int i = 0; i < int(A.size()); ++ i)
            assert(inner_product(A[i].begin(), A[i].end(), ans.x.begin(),
                                 0.L) <= b[i] + 1e-7L);
        assert(abs(inner_product(c.begin(), c.end(), ans.x.begin(),
                                 0.L) - ans.value) < 1e-7L);
    }
}

int main() {
    check_min25(); check_simplex();
    cout << "PASS: Min25 and Simplex randomized checks\n";
}
