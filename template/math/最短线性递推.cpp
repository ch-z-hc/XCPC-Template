/*
质数模下求序列前缀的最短递推及第 k 项，mod <= 2^31，输入系数先取模。
n 项求递推 O(n² + n log mod)，d 阶递推求第 k 项 O(d² log(k + 2))。
rec[i] 为前第 i + 1 项的系数。k 从 0 数，初值至少给出前 d 项。
*/

#include <bits/stdc++.h>

using i64 = long long;

static i64 bm_norm(i64 x, i64 mod) {
    x %= mod;
    return x < 0 ? x + mod : x;
}

static i64 bm_pow(i64 a, i64 b, i64 mod) {
    i64 res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

std::vector<i64> bm(const std::vector<i64> &s, i64 mod) {
    std::vector<i64> c{1}, b{1};
    int len = 0, m = 1;
    i64 last = 1;

    for (int n = 0; n < int(s.size()); n ++) {
        i64 d = s[n] % mod;
        for (int i = 1; i <= len; i ++) d = (d + c[i] * s[n - i]) % mod;
        if (d == 0) {
            ++ m;
            continue;
        }

        std::vector<i64> old = c;
        i64 coef = d * bm_pow(last, mod - 2, mod) % mod;
        if (int(c.size()) < int(b.size()) + m) c.resize(b.size() + m, 0);
        for (int i = 0; i < int(b.size()); i ++)
            c[i + m] = bm_norm(c[i + m] - coef * b[i], mod);

        if (2 * len <= n) {
            len = n + 1 - len;
            b.swap(old);
            last = d;
            m = 1;
        } else {
            ++ m;
        }
    }

    c.erase(c.begin());
    for (i64 &x : c) x = bm_norm(-x, mod);
    return c;
}

static std::vector<i64> bm_combine(const std::vector<i64> &a, const std::vector<i64> &b,
                                   const std::vector<i64> &rec, i64 mod) {
    int k = int(rec.size());
    std::vector<i64> ans(2 * k, 0);
    for (int i = 0; i < k; i ++)
        for (int j = 0; j < k; j ++) ans[i + j] = (ans[i + j] + a[i] * b[j]) % mod;
    for (int i = 2 * k - 2; i >= k; i --)
        if (ans[i])
            for (int j = 1; j <= k; j ++)
                ans[i - j] = (ans[i - j] + ans[i] * rec[j - 1]) % mod;
    ans.resize(k);
    return ans;
}

i64 kth(const std::vector<i64> &init, const std::vector<i64> &rec, i64 k, i64 mod) {
    int n = int(rec.size());
    if (k < int(init.size())) return bm_norm(init[int(k)], mod);
    if (n == 0) return 0;
    std::vector<i64> pol(n, 0), e(n, 0);
    pol[0] = 1;
    if (n == 1)
        e[0] = bm_norm(rec[0], mod);
    else
        e[1] = 1;
    while (k > 0) {
        if (k & 1) pol = bm_combine(pol, e, rec, mod);
        e = bm_combine(e, e, rec, mod);
        k >>= 1;
    }
    i64 ans = 0;
    for (int i = 0; i < n; i ++) ans = (ans + pol[i] * init[i]) % mod;
    return ans;
}

/*
std::vector<i64> s{0, 1, 1, 2, 3, 5}; auto rec = bm(s, 17);
i64 ans = kth({0, 1}, rec, 7, 17);
*/
