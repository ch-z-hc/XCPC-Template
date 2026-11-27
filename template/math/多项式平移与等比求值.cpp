/*
先复制「组合数」「多项式基本运算」。系数和 c 已按 998244353 取模，低次在前。
taylor_shift 求 f(x+c)，chirp_z 求 f(1), f(c), ..., f(c^(m-1))，含 c = 0。
shift_samples 将 f(0)..f(n-1) 平移到 f(c)..f(c+n-1)，次数 < n。
三者 O((n+m) log(n+m))；n、m <= 2^20。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

V < int > taylor_shift(const V < int > &a, int c) {
    int n = int(a.size()), mod = FPS::mod;
    if (!n) return {};
    Comb cb; cb.init(n, mod);
    V < int > f(n), g(n);
    ll power = 1;
    for (int i = 0; i < n; i ++) {
        f[n - 1 - i] = cb.fac[i] * a[i] % mod;
        g[i] = cb.ifac[i] * power % mod;
        power = power * c % mod;
    }
    auto h = FPS::mul(f, g);
    for (int i = 0; i < n; i ++) f[i] = h[n - 1 - i] * cb.ifac[i] % mod;
    return f;
}

V < int > chirp_z(const V < int > &a, int c, int m) {
    int n = int(a.size()), mod = FPS::mod;
    V < int > ans(m);
    if (!n || !m) return ans;
    if (!c) {
        fill(ans.begin(), ans.end(), a[0]);
        ans[0] = accumulate(a.begin(), a.end(), 0ll) % mod;
        return ans;
    }
    auto chirp = [mod](int len, int r) {
        V < int > f(len, 1);
        ll step = 1;
        for (int i = 1; i < len; i ++) {
            f[i] = step * f[i - 1] % mod;
            step = step * r % mod;
        }
        return f;
    };
    auto d = chirp(max(n, m), FPS::power(c, mod - 2));
    V < int > f(n);
    for (int i = 0; i < n; i ++) f[n - 1 - i] = 1ll * a[i] * d[i] % mod;
    auto h = FPS::mul(f, chirp(n + m - 1, c));
    for (int i = 0; i < m; i ++) ans[i] = 1ll * h[n - 1 + i] * d[i] % mod;
    return ans;
}

V < int > shift_samples(const V < int > &y, int c) {
    int n = int(y.size()), mod = FPS::mod;
    if (!n) return {};
    Comb cb; cb.init(n, mod);
    V < int > a(n), inv(2 * n - 1), pref(2 * n, 1);
    auto val = [&](int j) { return (c - n + 1 + j + mod) % mod; };
    for (int i = 0; i < n; i ++) {
        ll w = cb.ifac[i] * cb.ifac[n - 1 - i] % mod;
        if ((n - 1 - i) & 1) w = mod - w;
        a[i] = w * y[i] % mod;
    }
    for (int j = 0; j < 2 * n - 1; j ++)
        pref[j + 1] = 1ll * pref[j] * max(1, val(j)) % mod;
    ll back = FPS::power(pref.back(), mod - 2);
    for (int j = 2 * n - 2; j >= 0; j --) {
        inv[j] = val(j) ? back * pref[j] % mod : 0;
        back = back * max(1, val(j)) % mod;
    }
    auto h = FPS::mul(a, inv);
    V < int > ans(n);
    ll product = pref[n];
    for (int i = 0; i < n; i ++) {
        int x = (c + i) % mod;
        ans[i] = x < n ? y[x] : product * h[n - 1 + i] % mod;
        if (i + 1 < n) {
            product = product * max(1, inv[i]) % mod;
            product = product * max(1, val(i + n)) % mod;
        }
    }
    return ans;
}

/*
auto a = taylor_shift({1, 2, 3}, 4);
auto v = chirp_z({1, 2, 3}, 2, 6);
auto w = shift_samples({0, 1, 4}, 3);
*/
