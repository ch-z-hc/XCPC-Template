/*
先复制「组合数」。采样点为 0..n-1，次数 < n，值先取模，mod 为质数且 n < mod <= 2^31。
建表 O(n + log mod)，每次求值 O(n)，允许负坐标，空表视为零多项式。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct LagrangeEq {
    Comb comb;
    std::vector<i64> y;

    void build(const std::vector<i64> &values, i64 mod) {
        y = values;
        comb.init(int(y.size()), mod);
    }

    i64 query(i64 x) {
        int n = int(y.size());
        i64 mod = comb.mod;
        x %= mod;
        if (x < 0) x += mod;
        if (x < n) return y[int(x)];
        std::vector<i64> pre(n + 1, 1), suf(n + 1, 1);
        for (int i = 0; i < n; i ++) pre[i + 1] = pre[i] * ((x - i + mod) % mod) % mod;
        for (int i = n - 1; i >= 0; i --)
            suf[i] = suf[i + 1] * ((x - i + mod) % mod) % mod;
        i64 ans = 0;
        for (int i = 0; i < n; i ++) {
            i64 v = y[i] * pre[i] % mod * suf[i + 1] % mod;
            v = v * comb.ifac[i] % mod * comb.ifac[n - 1 - i] % mod;
            if ((n - 1 - i) & 1) v = (mod - v) % mod;
            ans = (ans + v) % mod;
        }
        return ans;
    }
};

/*
LagrangeEq poly; poly.build({0, 1, 4}, 1000000007);
i64 ans = poly.query(10);
*/
