/*
先复制「快速傅里叶变换与任意模数卷积」。用 15 位拆分及复数 FFT 求正 int 模数下的卷积。
时间 O(n log n)，空间 O(n)。浮点舍入依赖平台：建议 long double 有至少 64 位有效数字，
两数组长度均 <= 2^18；需要严格精确时用同章三模 NTT。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

V < int > split_conv(const V < int > &a, const V < int > &b, int mod) {
    if (a.empty() || b.empty()) return {};
    int len = int(a.size() + b.size() - 1), n = 1, base = 1 << 15;
    while (n < len) n *= 2;
    auto pack = [&](const V < int > &v) {
        V < Complex > f(n);
        for (int i = 0; i < int(v.size()); i ++) {
            int x = v[i] % mod;
            if (x < 0) x += mod;
            f[i] = Complex(x % base, x / base);
        }
        fft(f);
        return f;
    };
    auto x = pack(a), y = pack(b);
    V < Complex > ends(n), middle(n);
    for (int i = 0; i < n; i ++) {
        int j = (n - i) % n;
        Complex lo = (x[i] + conj(x[j])) * 0.5L;
        Complex hi = (x[i] - conj(x[j])) * Complex(0, -0.5L);
        Complex u = (y[i] + conj(y[j])) * 0.5L;
        Complex v = (y[i] - conj(y[j])) * Complex(0, -0.5L);
        ends[i] = lo * u + Complex(0, 1) * hi * v;
        middle[i] = hi * u + lo * v;
    }
    fft(ends, true);
    fft(middle, true);
    V < int > ans(len);
    for (int i = 0; i < len; i ++) {
        ll hi = llround(ends[i].imag()) % mod;
        ll mid = llround(middle[i].real()) % mod;
        ll lo = llround(ends[i].real()) % mod;
        ans[i] = ((hi * base + mid) % mod * base + lo) % mod;
    }
    return ans;
}

/*
auto c = split_conv({1, -2}, {3, 4}, 1000000007);
*/
