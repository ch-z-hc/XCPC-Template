#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 先复制数学章的“组合数”；已知次数 < n 的多项式在 0..n-1 的值。
// y 为 0-based，系数先归一化；mod 为质数，n < mod <= 2^31。
// build 为 O(n + log mod)，query 为 O(n)，空间 O(n)，支持重复 build。
// query 接受有符号 ll 横坐标；空采样视作零多项式。
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

// LagrangeEq poly; poly.build({0, 1, 4}, 1000000007);
// ll ans = poly.query(10); // 100。
