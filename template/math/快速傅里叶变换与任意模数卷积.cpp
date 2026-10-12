#include <bits/stdc++.h>
#include <cassert>

#define V vector

typedef long long ll;
typedef long double ld;

using namespace std;

typedef complex < ld > Complex;

// 卷积系数 0-based；时间 O(n log n)，空间 O(n)，空输入返回空数组。
// fft 长度为 2 的幂；fft_mul 是浮点 FFT，最后四舍五入到整数。
// 建议变换长度 <= 2^20，min(|a|, |b|) * max_abs(a) * max_abs(b) <= 10^12。
// 上述是保守使用范围，不是严格精度保证；误差依赖平台的 long double。
// 严格模卷积用 conv_mod：1 <= mod <= 2^31 - 1，支持负系数。
// conv_mod 用三个 NTT + CRT，变换长度 <= 2^21（受 1004535809 限制）。
// 此范围内整数卷积小于三模数之积，CRT 精确，不依赖浮点舍入。

void fft(V < Complex > &a, bool inv = false) {
    int n = int(a.size());
    for (int i = 1, j = 0; i < n; ++ i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    const ld pi = acosl(-1);
    for (int len = 2; len <= n; len <<= 1) {
        ld angle = 2 * pi / len * (inv ? -1 : 1);
        Complex wn(cosl(angle), sinl(angle));
        for (int l = 0; l < n; l += len) {
            Complex w(1, 0);
            for (int i = 0; i < len / 2; ++ i, w *= wn) {
                Complex x = a[l + i], y = w * a[l + i + len / 2];
                a[l + i] = x + y;
                a[l + i + len / 2] = x - y;
            }
        }
    }
    if (inv) for (auto &x : a) x /= n;
}

V < ll > fft_mul(const V < ll > &a, const V < ll > &b) {
    if (a.empty() || b.empty()) return {};
    int m = int(a.size() + b.size() - 1), n = 1;
    while (n < m) n <<= 1;
    V < Complex > x(n), y(n);
    for (int i = 0; i < int(a.size()); ++ i) x[i] = a[i];
    for (int i = 0; i < int(b.size()); ++ i) y[i] = b[i];
    fft(x); fft(y);
    for (int i = 0; i < n; ++ i) x[i] *= y[i];
    fft(x, true);
    V < ll > ans(m);
    for (int i = 0; i < m; ++ i) ans[i] = llround(x[i].real());
    return ans;
}

struct ModConv {
    static int power(int a, int b, int mod) {
        int ans = 1;
        for (; b; b >>= 1, a = 1ll * a * a % mod)
            if (b & 1) ans = 1ll * ans * a % mod;
        return ans;
    }

    static void ntt(V < int > &a, int mod, bool inv = false) {
        int n = int(a.size());
        for (int i = 1, j = 0; i < n; ++ i) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            int wn = power(3, (mod - 1) / len, mod);
            if (inv) wn = power(wn, mod - 2, mod);
            for (int l = 0; l < n; l += len) {
                ll w = 1;
                for (int i = 0; i < len / 2; ++ i, w = w * wn % mod) {
                    int x = a[l + i], y = w * a[l + i + len / 2] % mod;
                    a[l + i] = x + y;
                    if (a[l + i] >= mod) a[l + i] -= mod;
                    a[l + i + len / 2] = x - y;
                    if (a[l + i + len / 2] < 0) a[l + i + len / 2] += mod;
                }
            }
        }
        if (inv) {
            int v = power(n, mod - 2, mod);
            for (int &x : a) x = 1ll * x * v % mod;
        }
    }

    static V < int > mul(const V < int > &a, const V < int > &b, int mod) {
        int m = int(a.size() + b.size() - 1), n = 1;
        while (n < m) n <<= 1;
        V < int > x(n), y(n);
        for (int i = 0; i < int(a.size()); ++ i) x[i] = a[i] % mod;
        for (int i = 0; i < int(b.size()); ++ i) y[i] = b[i] % mod;
        ntt(x, mod); ntt(y, mod);
        for (int i = 0; i < n; ++ i) x[i] = 1ll * x[i] * y[i] % mod;
        ntt(x, mod, true);
        x.resize(m);
        return x;
    }
};

V < int > conv_mod(const V < ll > &a, const V < ll > &b, int mod) {
    assert(mod >= 1);
    if (a.empty() || b.empty()) return {};
    assert(a.size() + b.size() - 1 <= (1 << 21));
    V < int > x(a.size()), y(b.size());
    for (int i = 0; i < int(a.size()); ++ i) x[i] = (a[i] % mod + mod) % mod;
    for (int i = 0; i < int(b.size()); ++ i) y[i] = (b[i] % mod + mod) % mod;
    const int p = 998244353, q = 1004535809, r = 469762049;
    V < int > c = ModConv::mul(x, y, p);
    V < int > d = ModConv::mul(x, y, q), e = ModConv::mul(x, y, r);
    ll v = ModConv::power(p, q - 2, q);
    ll w = ModConv::power(1ll * p * q % r, r - 2, r);
    for (int i = 0; i < int(c.size()); ++ i) {
        ll t = (d[i] - c[i] + q) * v % q;
        ll u = (e[i] - (c[i] + 1ll * p * t) % r + r) * w % r;
        c[i] = (c[i] + 1ll * p * t % mod + (1ll * p * q % mod) * u % mod) % mod;
    }
    return c;
}

// 调用示例：
// V < ll > a{1, -2, 3}, b{4, 5};
// auto c = fft_mul(a, b);
// auto d = conv_mod(a, b, 1000000007);
