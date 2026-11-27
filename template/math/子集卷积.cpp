/*
求 c[S] = Σ a[T] * b[S ^ T]，T 枚举 S 的子集。数组等长，长度为 2^n，0 <= n <= 20。
模数可取任意正 int，系数可负。时间 O(n² 2^n)，空间 O(n 2^n)，n = 20 时主要数组约占 180 MiB。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

V < int > subset_conv(const V < int > &a, const V < int > &b, int mod) {
    int size = int(a.size()), n = __builtin_ctz(unsigned(size));
    V < int > cnt(size), ans(size), c(size);
    V < V < int > > f(n + 1, V < int >(size)), g = f;
    for (int s = 0; s < size; s ++) {
        cnt[s] = __builtin_popcount(unsigned(s));
        int x = a[s] % mod, y = b[s] % mod;
        if (x < 0) x += mod;
        if (y < 0) y += mod;
        f[cnt[s]][s] = x;
        g[cnt[s]][s] = y;
    }
    auto transform = [&](V < int > &row, bool inverse) {
        for (int bit = 1; bit < size; bit <<= 1)
            for (int l = 0; l < size; l += bit * 2)
                for (int i = 0; i < bit; i ++) {
                    ll x = row[l + bit + i];
                    x += inverse ? -row[l + i] : row[l + i];
                    if (x < 0) x += mod;
                    if (x >= mod) x -= mod;
                    row[l + bit + i] = int(x);
                }
    };
    for (int k = 0; k <= n; k ++) {
        transform(f[k], false); transform(g[k], false);
    }
    for (int k = 0; k <= n; k ++) {
        fill(c.begin(), c.end(), 0);
        for (int j = 0; j <= k; j ++)
            for (int s = 0; s < size; s ++)
                c[s] = (c[s] + 1ll * f[j][s] * g[k - j][s]) % mod;
        transform(c, true);
        for (int s = 0; s < size; s ++)
            if (cnt[s] == k) ans[s] = c[s];
    }
    return ans;
}

// auto c = subset_conv(a, b, 998244353);
