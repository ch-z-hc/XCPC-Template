#include <bits/stdc++.h>
#include "../../template/math/多项式基本运算.cpp"
#include "../../template/math/快速傅里叶变换与任意模数卷积.cpp"

typedef __int128_t i128; // 独立卷积真值，不属于比赛模板。

constexpr int P = 998244353;

int ref_pow(int a, int b) {
    ll x = a, ans = 1;
    while (b) {
        if (b & 1) ans = ans * x % P;
        x = x * x % P; b >>= 1;
    }
    return int(ans);
}

V < int > ref_mul(const V < int > &a, const V < int > &b) {
    if (a.empty() || b.empty()) return {};
    V < int > c(a.size() + b.size() - 1);
    for (int i = 0; i < int(a.size()); ++ i)
        for (int j = 0; j < int(b.size()); ++ j)
            c[i + j] = (c[i + j] + 1ll * a[i] * b[j]) % P;
    return c;
}

V < int > ref_inv(const V < int > &a, int n) {
    V < int > b(n);
    if (! n) return b;
    b[0] = ref_pow(a[0], P - 2);
    for (int i = 1; i < n; ++ i) {
        ll sum = 0;
        for (int j = 1; j <= i && j < int(a.size()); ++ j)
            sum = (sum + 1ll * a[j] * b[i - j]) % P;
        b[i] = (P - sum) * b[0] % P;
    }
    return b;
}

V < int > ref_ln(const V < int > &a, int n) {
    V < int > b(n), inv = ref_inv(a, n);
    for (int i = 1; i < n; ++ i) {
        ll sum = 0;
        for (int j = 1; j <= i && j < int(a.size()); ++ j)
            sum = (sum + 1ll * j * a[j] % P * inv[i - j]) % P;
        b[i] = sum * ref_pow(i, P - 2) % P;
    }
    return b;
}

V < int > ref_exp(const V < int > &a, int n) {
    V < int > b(n);
    if (! n) return b;
    b[0] = 1;
    for (int i = 1; i < n; ++ i) {
        ll sum = 0;
        for (int j = 1; j <= i && j < int(a.size()); ++ j)
            sum = (sum + 1ll * j * a[j] % P * b[i - j]) % P;
        b[i] = sum * ref_pow(i, P - 2) % P;
    }
    return b;
}

void test_fps(mt19937_64 &rng) {
    assert(FPS::mul({}, {1}).empty());
    assert(FPS::inv({}, 0).empty() && FPS::ln({}, 0).empty());
    assert(FPS::exp({}, 0).empty());
    assert(FPS::inv({5}, 1) == (V < int >{ref_pow(5, P - 2)}));
    assert(FPS::ln({1}, 1) == (V < int >{0}));
    assert(FPS::exp({0}, 1) == (V < int >{1}));
    assert(FPS::exp({}, 9) == (V < int >{1, 0, 0, 0, 0, 0, 0, 0, 0}));
    for (int t = 0; t < 350; ++ t) {
        int n = rng() % 100, len = rng() % 100 + 1, m = rng() % 100;
        V < int > a(len), b(m);
        for (int &x : a) x = rng() % P;
        for (int &x : b) x = rng() % P;
        assert(FPS::mul(a, b) == ref_mul(a, b));
        a[0] = rng() % (P - 1) + 1;
        assert(FPS::inv(a, n) == ref_inv(a, n));
        a[0] = 1;
        assert(FPS::ln(a, n) == ref_ln(a, n));
        auto c = FPS::exp(FPS::ln(a, n), n);
        auto truncated = a; truncated.resize(n);
        assert(c == truncated);
        a[0] = 0;
        assert(FPS::exp(a, n) == ref_exp(a, n));
    }
    for (int n : {1, 2, 3, 7, 8, 9, 31, 32, 33, 63, 64, 65, 129}) {
        V < int > a(n);
        for (int &x : a) x = rng() % P;
        a[0] = 1;
        assert(FPS::inv(a, n) == ref_inv(a, n));
        assert(FPS::ln(a, n) == ref_ln(a, n));
        a[0] = 0;
        assert(FPS::exp(a, n) == ref_exp(a, n));
    }
}

V < int > ref_conv(const V < ll > &a, const V < ll > &b, int mod) {
    if (a.empty() || b.empty()) return {};
    V < int > c(a.size() + b.size() - 1);
    for (int i = 0; i < int(a.size()); ++ i)
        for (int j = 0; j < int(b.size()); ++ j) {
            ll x = (a[i] % mod + mod) % mod;
            ll y = (b[j] % mod + mod) % mod;
            c[i + j] = (c[i + j] + i128(x) * y) % mod;
        }
    return c;
}

void test_fft(mt19937_64 &rng) {
    for (int n : {1, 2, 4, 8, 16, 32, 64, 128}) {
        V < Complex > a(n);
        for (auto &x : a)
            x = Complex(int(rng() % 101) - 50, int(rng() % 101) - 50);
        auto b = a;
        fft(b);
        if (n <= 16) for (int i = 0; i < n; ++ i) {
            Complex expected;
            for (int j = 0; j < n; ++ j) {
                ld angle = 2 * acosl(-1) * i * j / n;
                expected += a[j] * Complex(cosl(angle), sinl(angle));
            }
            assert(abs(expected - b[i]) < 1e-8L);
        }
        fft(b, true);
        for (int i = 0; i < n; ++ i) assert(abs(a[i] - b[i]) < 1e-8L);
    }
    assert(fft_mul({}, {1}).empty());
    V < ll > edge(64, 125000), expected(127);
    for (int i = 0; i < 64; ++ i)
        for (int j = 0; j < 64; ++ j) expected[i + j] += edge[i] * edge[j];
    assert(fft_mul(edge, edge) == expected);
    for (int t = 0; t < 300; ++ t) {
        int n = rng() % 100 + 1, m = rng() % 100 + 1;
        V < ll > a(n), b(m), c(n + m - 1);
        for (ll &x : a) x = ll(rng() % 20001) - 10000;
        for (ll &x : b) x = ll(rng() % 20001) - 10000;
        for (int i = 0; i < n; ++ i)
            for (int j = 0; j < m; ++ j) c[i + j] += a[i] * b[j];
        assert(fft_mul(a, b) == c);
    }
}

void test_mod_conv(mt19937_64 &rng) {
    const V < int > mods{1, 2, 998244353, 1000000007, INT_MAX};
    for (int mod : mods) {
        assert(conv_mod({}, {LLONG_MIN}, mod).empty());
        for (int n : {1, 2, 3, 7, 8, 9, 31, 32, 33, 64, 65, 129}) {
            int m = n + 2;
            V < ll > a(n), b(m);
            for (int i = 0; i < n; ++ i)
                a[i] = i % 3 == 0 ? LLONG_MIN : i % 3 == 1 ? LLONG_MAX : -1;
            for (int i = 0; i < m; ++ i)
                b[i] = i % 2 ? LLONG_MIN : LLONG_MAX;
            assert(conv_mod(a, b, mod) == ref_conv(a, b, mod));
        }
    }
    for (int t = 0; t < 180; ++ t) {
        int n = rng() % 80 + 1, m = rng() % 80 + 1;
        int mod = t % 2 ? mods[rng() % mods.size()] : int(rng() % INT_MAX) + 1;
        V < ll > a(n), b(m);
        for (ll &x : a) x = rng() & 1 ? -ll(rng() >> 1) : ll(rng() >> 1);
        for (ll &x : b) x = rng() & 1 ? -ll(rng() >> 1) : ll(rng() >> 1);
        assert(conv_mod(a, b, mod) == ref_conv(a, b, mod));
    }
}

int main() {
    mt19937_64 rng(998244353);
    test_fps(rng); test_fft(rng); test_mod_conv(rng);
    cout << "PASS: FPS, FFT and exact modular convolution random tests\n";
}
