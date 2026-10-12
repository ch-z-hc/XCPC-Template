#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 根据 0-based 序列前缀求最短线性递推式；s[n] = rec[0]s[n-1] + ...。
// mod 必须为质数；BM 复杂度 O(n^2)，d 阶递推第 k 项复杂度 O(d^2 log k)。
// 要求 1 < mod <= 2^31，输入模数值归一化到 [0, mod)，乘加计算不得溢出 ll。

static ll bm_norm(ll x, ll mod) {
    x %= mod;
    return x < 0 ? x + mod : x;
}

static ll bm_pow(ll a, ll b, ll mod) {
    ll res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

V < ll > bm(const V < ll > &s, ll mod) {
    V < ll > c{1}, b{1};
    int len = 0, m = 1;
    ll last = 1;

    for (int n = 0; n < int(s.size()); ++ n) {
        ll d = s[n] % mod;
        for (int i = 1; i <= len; ++ i)
            d = (d + c[i] * s[n - i]) % mod;
        if (d == 0) {
            ++ m;
            continue;
        }

        V < ll > old = c;
        ll coef = d * bm_pow(last, mod - 2, mod) % mod;
        if (int(c.size()) < int(b.size()) + m) c.resize(b.size() + m, 0);
        for (int i = 0; i < int(b.size()); ++ i)
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
    for (ll &x : c) x = bm_norm(-x, mod);
    return c;
}

static V < ll > bm_combine(const V < ll > &a, const V < ll > &b,
                          const V < ll > &rec, ll mod) {
    int k = int(rec.size());
    V < ll > ans(2 * k, 0);
    for (int i = 0; i < k; ++ i)
        for (int j = 0; j < k; ++ j)
            ans[i + j] = (ans[i + j] + a[i] * b[j]) % mod;
    for (int i = 2 * k - 2; i >= k; -- i) if (ans[i])
        for (int j = 1; j <= k; ++ j)
            ans[i - j] = (ans[i - j] + ans[i] * rec[j - 1]) % mod;
    ans.resize(k);
    return ans;
}

// 求 0-based 的第 k 项；初始序列长度应至少等于递推阶数。
ll kth(const V < ll > &init, const V < ll > &rec,
                         ll k, ll mod) {
    int n = int(rec.size());
    if (k < int(init.size())) return bm_norm(init[int(k)], mod);
    if (n == 0) return 0;
    V < ll > pol(n, 0), e(n, 0);
    pol[0] = 1;
    if (n == 1) e[0] = bm_norm(rec[0], mod);
    else e[1] = 1;
    while (k > 0) {
        if (k & 1) pol = bm_combine(pol, e, rec, mod);
        e = bm_combine(e, e, rec, mod);
        k >>= 1;
    }
    ll ans = 0;
    for (int i = 0; i < n; ++ i) ans = (ans + pol[i] * init[i]) % mod;
    return ans;
}

// 调用示例：
// V < ll > rec = bm(s, mod);
// ll ans = kth(init, rec, k, mod);
