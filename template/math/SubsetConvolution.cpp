#include <bits/stdc++.h>

#define V vector

using namespace std;

// 子集卷积：c[S] = sum(a[T] * b[S \ T])，T 为 S 的子集。
// 数组按 0-based 位掩码编号，长度相同且为 2^n，0 <= n <= 20。
// 1 <= mod <= INT_MAX，可为合数；系数允许为负，结果归一化。
// 分层子集 zeta / Mobius 变换，时间 O(n^2 2^n)，空间 O(n 2^n)。
// n = 20 时主要数组约占 180 MiB；不是 FWT 的 OR 卷积。
V < int > subset_convolution(const V < int > &a, const V < int > &b,
                              int mod) {
    int size = int(a.size()), n = __builtin_ctz(unsigned(size));
    V < int > cnt(size), ans(size), c(size);
    V < V < int > > f(n + 1, V < int > (size)), g = f;
    for (int s = 0; s < size; ++ s) {
        cnt[s] = __builtin_popcount(unsigned(s));
        int x = a[s] % mod, y = b[s] % mod;
        if (x < 0) x += mod;
        if (y < 0) y += mod;
        f[cnt[s]][s] = x; g[cnt[s]][s] = y;
    }
    auto transform = [&](V < int > &row, bool inverse) {
        for (int bit = 1; bit < size; bit <<= 1)
            for (int l = 0; l < size; l += bit * 2)
                for (int i = 0; i < bit; ++ i) {
                    long long x = row[l + bit + i];
                    x += inverse ? -row[l + i] : row[l + i];
                    if (x < 0) x += mod;
                    if (x >= mod) x -= mod;
                    row[l + bit + i] = int(x);
                }
    };
    for (int k = 0; k <= n; ++ k) {
        transform(f[k], false); transform(g[k], false);
    }
    for (int k = 0; k <= n; ++ k) {
        fill(c.begin(), c.end(), 0);
        for (int j = 0; j <= k; ++ j)
            for (int s = 0; s < size; ++ s)
                c[s] = (c[s] + 1ll * f[j][s] * g[k - j][s]) % mod;
        transform(c, true);
        for (int s = 0; s < size; ++ s) if (cnt[s] == k) ans[s] = c[s];
    }
    return ans;
}

// 调用：auto c = subset_convolution(a, b, 998244353);
