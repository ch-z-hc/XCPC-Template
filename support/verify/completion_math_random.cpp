#include "../../template/math/组合数.cpp"
#include "../../template/math/等距拉格朗日插值.cpp"
#include "../../template/math/约数与倍数变换与卷积.cpp"
#include "../../template/math/快速数论变换与多项式乘法.cpp"
#include "../../template/math/两类斯特林数.cpp"
#include "../../template/math/多项式基本运算.cpp"
#include "../../template/math/线性递推快速求项.cpp"

ll norm(ll x, ll mod) { x %= mod; return x < 0 ? x + mod : x; }

ll evaluate(const V < ll > &a, ll x, ll mod) {
    ll ans = 0; x = norm(x, mod);
    for (int i = int(a.size()) - 1; i >= 0; -- i) ans = (ans * x + a[i]) % mod;
    return ans;
}

void check_lagrange(mt19937 &rng) {
    LagrangeEq poly;
    for (int rep = 0; rep < 2000; ++ rep) {
        ll mod = V < ll > {2, 17, 101, 1000000007, 2147483647}[rep % 5];
        int n = int(min(ll(rng() % 25), mod - 1));
        V < ll > a(n), y(n);
        for (ll &v : a) v = rng() % mod;
        for (int i = 0; i < n; ++ i) y[i] = evaluate(a, i, mod);
        poly.build(y, mod);
        for (int i = 0; i < n; ++ i) assert(poly.query(i) == y[i]);
        for (ll x : {LLONG_MIN, LLONG_MAX, -1ll, ll(rng())})
            assert(poly.query(x) == evaluate(a, x, mod));
    }
    V < ll > y(100000);
    for (int i = 0; i < int(y.size()); ++ i) y[i] = 1ll * i * i % 1000000007;
    poly.build(y, 1000000007);
    assert(poly.query(100001) == 100001ll * 100001 % 1000000007);
    poly.build({}, 17); assert(poly.query(LLONG_MIN) == 0);
}

void check_divisors(mt19937 &rng) {
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = int(rng() % 40);
        ll mod = V < ll > {1, 8, 97, 1000000007, 2147483648ll}[rep % 5];
        V < ll > a(n + 1), b(n + 1), d(n + 1), m(n + 1);
        V < ll > cg(n + 1), cl(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = rng() % mod, b[i] = rng() % mod;
        for (int i = 1; i <= n; ++ i) for (int j = 1; j <= n; ++ j) {
            if (i % j == 0) d[i] = (d[i] + a[j]) % mod;
            if (j % i == 0) m[i] = (m[i] + a[j]) % mod;
            int g = gcd(i, j), l = i / g * j;
            cg[g] = (cg[g] + a[i] * b[j]) % mod;
            if (l <= n) cl[l] = (cl[l] + a[i] * b[j]) % mod;
        }
        auto c = a; divisor_zeta(c, mod); assert(c == d);
        divisor_zeta(c, mod, true); assert(c == a);
        c = a; multiple_zeta(c, mod); assert(c == m);
        multiple_zeta(c, mod, true); assert(c == a);
        assert(gcd_conv(a, b, mod) == cg);
        assert(lcm_conv(a, b, mod) == cl);
    }
}

void check_stirling(mt19937 &rng) {
    for (int n = 0; n <= 8; ++ n) {
        V < ll > first(n + 1), second(n + 1);
        V < int > a(n); iota(a.begin(), a.end(), 0);
        do {
            V < bool > seen(n, false);
            int cnt = 0;
            for (int i = 0; i < n; ++ i) if (! seen[i]) {
                ++ cnt;
                for (int j = i; ! seen[j]; j = a[j]) seen[j] = true;
            }
            ++ first[cnt];
        } while (next_permutation(a.begin(), a.end()));
        auto partition = [&](auto &&self, int i, int used) -> void {
            if (i == n) { ++ second[used]; return; }
            for (int j = 0; j <= used; ++ j) self(self, i + 1, max(used, j + 1));
        };
        partition(partition, 0, 0);
        assert(stirling1(n, 1000000007)[n] == first);
        assert(stirling2(n, 1000000007)[n] == second);
        assert(stirling2_row(n) == second);
    }
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = int(rng() % 70);
        ll mod = V < ll > {1, 8, 97, 2147483648ll}[rep % 4];
        auto s = stirling1(n, mod), t = stirling2(n, mod);
        ll x = rng() % mod, power = 1 % mod, falling = 1 % mod, rising = 1 % mod;
        V < ll > f(n + 1, 1 % mod), p(n + 1, 1 % mod);
        for (int i = 1; i <= n; ++ i) {
            p[i] = p[i - 1] * x % mod;
            f[i] = f[i - 1] * norm(x - i + 1, mod) % mod;
            rising = rising * ((x + i - 1) % mod) % mod;
        }
        power = p[n]; falling = 0;
        ll sum = 0;
        for (int i = 0; i <= n; ++ i) {
            sum = (sum + s[n][i] * p[i]) % mod;
            falling = (falling + t[n][i] * f[i]) % mod;
        }
        assert(sum == rising && falling == power);
        assert(stirling2_row(n) == stirling2(n, NTT_MOD)[n]);
    }
    auto row = stirling2_row(4096);
    assert(row[1] == 1 && row.back() == 1 && row[4095] == 4096ll * 4095 / 2);
}

pair < int, int > fibonacci(ull n) {
    if (! n) return {0, 1};
    auto [a, b] = fibonacci(n >> 1);
    int c = 1ll * a * norm(2ll * b - a, FPS::mod) % FPS::mod;
    int d = (1ll * a * a + 1ll * b * b) % FPS::mod;
    return n & 1 ? make_pair(d, (c + d) % FPS::mod) : make_pair(c, d);
}

void check_bostan(mt19937 &rng) {
    const int mod = FPS::mod;
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = int(rng() % 18), m = 1 + int(rng() % 15), k = int(rng() % 100);
        V < int > p(n), q(m), seq(k + 1);
        for (int &v : p) v = int(rng() % mod);
        for (int &v : q) v = int(rng() % mod);
        q[0] = 1 + int(rng() % (mod - 1));
        ll inv = FPS::power(q[0], mod - 2);
        for (int i = 0; i <= k; ++ i) {
            ll v = i < n ? p[i] : 0;
            for (int j = 1; j < m && j <= i; ++ j)
                v = norm(v - 1ll * q[j] * seq[i - j], mod);
            seq[i] = int(v * inv % mod);
        }
        assert(bostan_mori(p, q, k) == seq[k]);
        int d = int(rng() % 15);
        V < int > init(d), rec(d), a(max(k + 1, d));
        for (int i = 0; i < d; ++ i) init[i] = a[i] = int(rng() % mod);
        for (int &v : rec) v = int(rng() % mod);
        for (int i = d; i <= k; ++ i) for (int j = 0; j < d; ++ j)
            a[i] = (a[i] + 1ll * rec[j] * a[i - 1 - j]) % mod;
        assert(linear_kth(init, rec, k) == a[k]);
    }
    for (ull k : {0ull, 1ull, 1000000000000000000ull, ULLONG_MAX}) {
        assert(linear_kth({0, 1}, {1, 1}, k) == fibonacci(k).first);
        assert(bostan_mori({1}, {1, mod - 1}, k) == 1);
        assert(bostan_mori({}, {2}, k) == 0);
    }
    const int d = 2048, k = 4096;
    V < int > init(d, 1), rec(d, 1), a(k + 1, 1);
    for (int i = d; i <= k; ++ i) {
        a[i] = 0;
        for (int j = 1; j <= d; ++ j) a[i] = (a[i] + a[i - j]) % mod;
    }
    assert(linear_kth(init, rec, k) == a[k]);
}

int main() {
    mt19937 rng(20261011);
    check_lagrange(rng); check_divisors(rng); check_stirling(rng); check_bostan(rng);
    cout << "PASS: interpolation, divisor transforms, Stirling, Bostan-Mori\n";
}
