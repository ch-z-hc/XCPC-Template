#include <cassert>
#include "../../template/math/素性测试与整数分解.cpp"
#include "../../template/math/高斯消元.cpp"
#include "../../template/math/扩展欧几里得与中国剩余定理.cpp"
#include "../../template/math/离散对数.cpp"
#include "../../template/math/卢卡斯定理.cpp"
#include "../../template/math/线性筛积性函数.cpp"

// 独立宽整数真值，仅用于验证，不属于比赛模板。
typedef __uint128_t u128;
typedef __int128_t CRTInt;

void check_factor(ull n) {
    auto a = factor(n);
    assert(is_sorted(a.begin(), a.end()));
    u128 prod = 1;
    for (ull p : a) { assert(is_prime(p)); prod *= p; }
    if (n >= 2) assert(prod == n);
    else assert(a.empty());
}

void test_factor(mt19937_64 &rng) {
    for (int t = 0; t < 10000; ++ t) {
        ull mod = rng() | 1, a = rng(), b = rng();
        assert(rho_mul(a, b, mod) == u128(a) * b % mod);
    }
    for (ull n = 0; n < 20000; ++ n) {
        bool op = n >= 2;
        for (ull p = 2; p * p <= n; ++ p)
            if (n % p == 0) { op = 0; break; }
        assert(is_prime(n) == op);
    }
    for (ull n : {0ULL, 1ULL, 2ULL, 4ULL, 561ULL, 3215031751ULL,
                  3825123056546413051ULL, 9223372036854775808ULL,
                  18446744073709551615ULL, 18446744073709551557ULL,
                  18446743979220271189ULL}) check_factor(n);
    assert(is_prime(18446744073709551557ULL));
    assert(! is_prime(3825123056546413051ULL));
    for (int t = 0; t < 50; ++ t) check_factor(rng());
}

void test_gauss(mt19937_64 &rng) {
    for (int t = 0; t < 2000; ++ t) {
        int n = rng() % 5, m = rng() % 5, p = rng() % 2 ? 3 : 5;
        V < V < ll > > a(n, V < ll > (m + 1));
        V < V < ld > > b(n, V < ld > (m + 1));
        for (int i = 0; i < n; ++ i)
            for (int j = 0; j <= m; ++ j)
                a[i][j] = int(rng() % 11) - 5, b[i][j] = a[i][j];
        V < ll > x, z; V < ld > y;
        int op = gauss_mod(a, m, p, x), cnt = 0, ways = 1;
        for (int i = 0; i < m; ++ i) ways *= p;
        for (int mask = 0; mask < ways; ++ mask) {
            V < int > v(m); int k = mask;
            for (int j = 0; j < m; ++ j) v[j] = k % p, k /= p;
            bool ok = 1;
            for (int i = 0; i < n; ++ i) {
                ll res = -a[i][m];
                for (int j = 0; j < m; ++ j) res += a[i][j] * v[j];
                if (res % p) ok = 0;
            }
            cnt += ok;
        }
        assert(op == (cnt == 0 ? 0 : cnt == 1 ? 1 : 2));
        if (op) for (int i = 0; i < n; ++ i) {
            ll res = -a[i][m];
            for (int j = 0; j < m; ++ j) res += a[i][j] * x[j];
            assert(res % p == 0);
        }
        int q = gauss(b, m, y);
        // 小整数矩阵的所有子式远小于模数，可用模消元独立核对实数秩。
        assert(q == gauss_mod(a, m, 1000000007, z));
        if (q) for (int i = 0; i < n; ++ i) {
            ld res = -b[i][m];
            for (int j = 0; j < m; ++ j) res += b[i][j] * y[j];
            assert(abs(res) < 1e-8L);
        }
        V < bitset < 8 > > c(n); V < int > u;
        for (int i = 0; i < n; ++ i)
            for (int j = 0; j <= m; ++ j) c[i][j] = (a[i][j] % 2 + 2) % 2;
        int state = gauss_xor(c, m, u);
        assert(state == gauss_mod(a, m, 2, z));
        if (state) for (int j = 0; j < m; ++ j) assert(u[j] == z[j]);
    }
    V < ld > x; V < ll > y; V < int > z;
    assert(gauss({}, 0, x) == 1 && x.empty());
    assert(gauss({}, 3, x) == 2 && x == V < ld > (3, 0));
    assert(gauss({{1}}, 0, x) == 0);
    assert(gauss({{1e-15L, 0}}, 1, x) == 2);
    assert(gauss({{1e-15L, 1}}, 1, x) == 0);
    assert(gauss({{0, 2, 4, 6}}, 3, x) == 2);
    assert(x == (V < ld >{0, 3, 0}));
    assert(gauss_mod({}, 0, 2, y) == 1);
    assert(gauss_mod({{1}}, 0, 2, y) == 0);
    assert(gauss_mod({{0, 2, 4, 6}}, 3, 5, y) == 2);
    assert(y == (V < ll >{0, 3, 0}));
    assert(gauss_mod({{2147483646LL, 1}}, 1, 2147483647LL, y) == 1);
    assert(y[0] == 2147483646LL);
    assert(gauss_mod({{LLONG_MIN, LLONG_MIN}}, 1, 2147483647LL, y) == 1);
    assert(y[0] == 1);
    assert(gauss_xor(V < bitset < 1 > >{}, 0, z) == 1);
    assert(gauss_xor(V < bitset < 1 > >{bitset < 1 > (1)}, 0, z) == 0);
    assert(gauss_xor(V < bitset < 4 > >{bitset < 4 > (14)}, 3, z) == 2);
    assert(z == (V < int >{0, 1, 0}));
}

void test_crt(mt19937_64 &rng) {
    for (int t = 0; t < 10000; ++ t) {
        ll a = rng() & LLONG_MAX, b = rng() & LLONG_MAX, x, y;
        ll g = exgcd(a, b, x, y);
        assert(CRTInt(a) * x + CRTInt(b) * y == g && g == gcd(a, b));
        ll m = ll(rng() % LLONG_MAX) + 1;
        a %= m; b %= m;
        assert(crt_mul(a, b, m) == CRTInt(a) * b % m);
    }
    for (ll a : {0LL, 1LL, 2LL, LLONG_MAX - 1, LLONG_MAX})
        for (ll b : {0LL, 1LL, 2LL, LLONG_MAX - 1, LLONG_MAX}) {
            ll x, y, g = exgcd(a, b, x, y);
            assert(g == gcd(a, b));
            assert(CRTInt(a) * x + CRTInt(b) * y == g);
        }
    for (ll mod : {1LL, 2LL, 17LL, LLONG_MAX - 1, LLONG_MAX})
        for (ll a : {LLONG_MIN, -2LL, -1LL, 0LL, 1LL, 2LL, LLONG_MAX}) {
            auto v = inv(a, mod);
            ll norm = a % mod; if (norm < 0) norm += mod;
            assert((v != -1) == (gcd(norm, mod) == 1));
            if (v != -1) {
                assert(0 <= v && v < mod);
                assert(CRTInt(norm) * v % mod == 1 % mod);
            }
        }
    for (int t = 0; t < 2000; ++ t) {
        int n = rng() % 4; V < ll > r(n), m(n);
        ll mod = 1;
        for (int i = 0; i < n; ++ i) {
            m[i] = rng() % 8 + 1; r[i] = int(rng() % 41) - 20;
            mod = lcm(mod, m[i]);
        }
        ll expected = -1;
        for (ll x = 0; x < mod; ++ x) {
            bool ok = 1;
            for (int i = 0; i < n; ++ i)
                if ((x - r[i]) % m[i]) ok = 0;
            if (ok) { expected = x; break; }
        }
        ll ans, actual_mod;
        bool ok = crt(r, m, ans, actual_mod);
        assert(ok == (expected != -1));
        if (ok) assert(ans == expected && actual_mod == mod);
    }
    ll ans, mod;
    assert(crt({LLONG_MIN}, {LLONG_MAX}, ans, mod));
    assert(ans == LLONG_MAX - 1 && mod == LLONG_MAX);
    assert(crt({LLONG_MAX, LLONG_MIN}, {LLONG_MAX, 1}, ans, mod));
    assert(ans == 0 && mod == LLONG_MAX);
    assert(! crt({0, 1}, {LLONG_MAX - 1, 2}, ans, mod));
    assert(! crt({0, 0}, {LLONG_MAX, 2}, ans, mod));
}

void test_bsgs(mt19937_64 &rng) {
    for (int mod = 1; mod <= 100; ++ mod) {
        for (int a = 0; a < mod; ++ a) {
            V < ll > expected(mod, -1);
            int cur = 1 % mod;
            for (int x = 0; expected[cur] == -1; ++ x) {
                expected[cur] = x; cur = cur * a % mod;
            }
            for (int b = 0; b < mod; ++ b) {
                assert(exbsgs(a, b, mod) == expected[b]);
                if (gcd(a, mod) == 1) assert(bsgs(a, b, mod) == expected[b]);
            }
        }
    }
    for (int t = 0; t < 300; ++ t) {
        ll mod = rng() % 100 + 1;
        ll a = t % 2 ? LLONG_MIN : -ll(rng() % 1000);
        ll b = t % 3 ? LLONG_MAX : -ll(rng() % 1000);
        ll x = (a % mod + mod) % mod, y = (b % mod + mod) % mod;
        assert(exbsgs(a, b, mod) == exbsgs(x, y, mod));
    }
}

void test_sieve_lucas() {
    Sieve sieve;
    for (int n : {0, 1, 1000, 15, 1, 0, 500}) {
        sieve.build(n);
        assert(int(sieve.lp.size()) == n + 1);
        V < int > primes;
        for (int i = 1; i <= n; ++ i) {
            int phi = 0, mu = 1, x = i;
            for (int j = 1; j <= i; ++ j) phi += gcd(i, j) == 1;
            for (int p = 2; p * p <= x; ++ p) if (x % p == 0) {
                int cnt = 0;
                while (x % p == 0) x /= p, cnt ++;
                mu = cnt >= 2 ? 0 : -mu;
            }
            if (x > 1) mu = -mu;
            assert(sieve.phi[i] == phi && sieve.mu[i] == mu);
            if (i >= 2) {
                int p = 2; while (i % p) p ++;
                assert(sieve.lp[i] == p);
                if (p == i) primes.push_back(i);
            }
        }
        assert(sieve.prime == primes);
    }
    Lucas comb;
    for (int p : {2, 3, 5, 7, 11, 17, 29, 2}) {
        comb.init(p);
        V < V < int > > c(201, V < int > (202));
        c[0][0] = 1;
        for (int n = 1; n <= 200; ++ n) {
            c[n][0] = 1;
            for (int k = 1; k <= n; ++ k)
                c[n][k] = (c[n - 1][k] + c[n - 1][k - 1]) % p;
        }
        for (int n = 0; n <= 200; ++ n)
            for (int k = 0; k <= n + 1; ++ k)
                assert(comb.choose(n, k) == c[n][k]);
        assert(comb.choose(LLONG_MAX, 0) == 1);
        assert(comb.choose(LLONG_MAX, 1) == LLONG_MAX % p);
        assert(comb.choose(LLONG_MAX, LLONG_MAX) == 1);
        assert(comb.choose(LLONG_MAX, LLONG_MAX - 1) == LLONG_MAX % p);
        assert(comb.choose(-1, 0) == 0 && comb.choose(3, -1) == 0);
    }
}

int main() {
    mt19937_64 rng(1000000007);
    test_factor(rng); test_gauss(rng); test_crt(rng);
    test_bsgs(rng); test_sieve_lucas();
    cout << "PASS: new number theory and Gauss random tests\n";
}
