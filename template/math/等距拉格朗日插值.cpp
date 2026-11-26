/*
先复制「组合数」。采样点为 0..n-1，次数 < n，值先取模，mod 为质数且 n < mod <= 2^31。
建表 O(n + log mod)，每次求值 O(n)，允许负坐标，空表视为零多项式。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct LagrangeEq {
    Comb comb;
    V < ll > y;

    void build(const V < ll > &values, ll mod) {
        y = values;
        comb.init(int(y.size()), mod);
    }

    ll query(ll x) {
        int n = int(y.size());
        ll mod = comb.mod;
        x %= mod; if (x < 0) x += mod;
        if (x < n) return y[int(x)];
        V < ll > pre(n + 1, 1), suf(n + 1, 1);
        for (int i = 0; i < n; ++ i)
            pre[i + 1] = pre[i] * ((x - i + mod) % mod) % mod;
        for (int i = n - 1; i >= 0; -- i)
            suf[i] = suf[i + 1] * ((x - i + mod) % mod) % mod;
        ll ans = 0;
        for (int i = 0; i < n; ++ i) {
            ll v = y[i] * pre[i] % mod * suf[i + 1] % mod;
            v = v * comb.ifac[i] % mod * comb.ifac[n - 1 - i] % mod;
            if ((n - 1 - i) & 1) v = (mod - v) % mod;
            ans = (ans + v) % mod;
        }
        return ans;
    }
};

/*
LagrangeEq poly; poly.build({0, 1, 4}, 1000000007);
ll ans = poly.query(10);
*/
