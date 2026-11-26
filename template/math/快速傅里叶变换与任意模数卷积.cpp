/*
整数卷积可用浮点 FFT，精确模卷积用三模 NTT。低次系数在前，变换长度 n 为 2 的幂，时间 O(n log n)。
浮点版建议 n <= 2^20 且 min(|a|, |b|) * max|a[i]| * max|b[i]| <= 10^12，精度依赖平台。
精确版支持负系数和任意正 int 模数，要求补长后 n <= 2^21。
*/

#include <bits/stdc++.h>

using i64 = long long;
using ld = long double;

using Complex = std::complex<ld>;

void fft(std::vector<Complex> &a, bool inv = false) {
    int n = int(a.size());
    if (inv && n > 1) std::reverse(a.begin() + 1, a.end());
    for (int i = 1, j = 0; i < n; i ++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(a[i], a[j]);
    }
    static std::vector<Complex> roots{0, 1};
    for (int k = int(roots.size()); k < n; k *= 2) {
        Complex w = std::polar(1.L, std::acos(-1.L) / k);
        roots.resize(2 * k);
        for (int j = k / 2; j < k; j ++) {
            roots[2 * j] = roots[j];
            roots[2 * j + 1] = roots[j] * w;
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int l = 0; l < n; l += 2 * k) {
            for (int j = 0; j < k; j ++) {
                Complex x = a[l + j], y = a[l + k + j] * roots[k + j];
                a[l + j] = x + y;
                a[l + k + j] = x - y;
            }
        }
    }
    if (inv)
        for (auto &x : a) x /= n;
}

std::vector<i64> fft_mul(const std::vector<i64> &a, const std::vector<i64> &b) {
    if (a.empty() || b.empty()) return {};
    int m = int(a.size() + b.size() - 1), n = 1;
    while (n < m) n <<= 1;
    std::vector<Complex> x(n), y(n);
    for (int i = 0; i < int(a.size()); i ++) x[i] = a[i];
    for (int i = 0; i < int(b.size()); i ++) y[i] = b[i];
    fft(x);
    fft(y);
    for (int i = 0; i < n; i ++) x[i] *= y[i];
    fft(x, true);
    std::vector<i64> ans(m);
    for (int i = 0; i < m; i ++) ans[i] = std::llround(x[i].real());
    return ans;
}

struct ModConv {
    static int power(int a, int b, int mod) {
        int ans = 1;
        for (; b; b >>= 1, a = 1ll * a * a % mod)
            if (b & 1) ans = 1ll * ans * a % mod;
        return ans;
    }

    static void ntt(std::vector<int> &a, int mod, bool inv = false) {
        int n = int(a.size());
        for (int i = 1, j = 0; i < n; i ++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) std::swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            int wn = power(3, (mod - 1) / len, mod);
            if (inv) wn = power(wn, mod - 2, mod);
            for (int l = 0; l < n; l += len) {
                i64 w = 1;
                for (int i = 0; i < len / 2; i ++, w = w * wn % mod) {
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

    static std::vector<int> mul(const std::vector<int> &a, const std::vector<int> &b,
                                int mod) {
        int m = int(a.size() + b.size() - 1), n = 1;
        while (n < m) n <<= 1;
        std::vector<int> x(n), y(n);
        for (int i = 0; i < int(a.size()); i ++) x[i] = a[i] % mod;
        for (int i = 0; i < int(b.size()); i ++) y[i] = b[i] % mod;
        ntt(x, mod);
        ntt(y, mod);
        for (int i = 0; i < n; i ++) x[i] = 1ll * x[i] * y[i] % mod;
        ntt(x, mod, true);
        x.resize(m);
        return x;
    }
};

std::vector<int> conv_mod(const std::vector<i64> &a, const std::vector<i64> &b, int mod) {
    assert(mod >= 1);
    if (a.empty() || b.empty()) return {};
    assert(a.size() + b.size() - 1 <= (1 << 21));
    std::vector<int> x(a.size()), y(b.size());
    for (int i = 0; i < int(a.size()); i ++) x[i] = (a[i] % mod + mod) % mod;
    for (int i = 0; i < int(b.size()); i ++) y[i] = (b[i] % mod + mod) % mod;
    const int p = 998244353, q = 1004535809, r = 469762049;
    std::vector<int> c = ModConv::mul(x, y, p);
    std::vector<int> d = ModConv::mul(x, y, q), e = ModConv::mul(x, y, r);
    i64 v = ModConv::power(p, q - 2, q);
    i64 w = ModConv::power(1ll * p * q % r, r - 2, r);
    for (int i = 0; i < int(c.size()); i ++) {
        i64 t = (d[i] - c[i] + q) * v % q;
        i64 u = (e[i] - (c[i] + 1ll * p * t) % r + r) * w % r;
        c[i] = (c[i] + 1ll * p * t % mod + (1ll * p * q % mod) * u % mod) % mod;
    }
    return c;
}

/*
std::vector<Complex> v{1, 2, 3, 0}; fft(v); fft(v, true);
std::vector<i64> a{1, -2, 3}, b{4, 5};
auto c = fft_mul(a, b); auto d = conv_mod(a, b, 1000000007);
*/
