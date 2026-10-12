#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 质数模数下的组合数，要求 0 <= n < mod；表下标范围 0..n。
// 预处理 O(n)，查询 O(1)，空间 O(n)。
// 要求 1 < mod <= 2^31，输入模数值归一化到 [0, mod)，乘加计算不得溢出 ll。

struct Comb {
    ll mod = 1;
    V < ll > fac, ifac;

    void init(int n, ll mod_) {
        mod = mod_;
        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);
        for (int i = 1; i <= n; ++ i) fac[i] = fac[i - 1] * i % mod;
        auto power = [&](ll a, ll b) {
            ll res = 1;
            for (; b; b >>= 1) {
                if (b & 1) res = 1ll * res * a % mod;
                a = 1ll * a * a % mod;
            }
            return res;
        };
        ifac[n] = power(fac[n], mod - 2);
        for (int i = n; i >= 1; -- i) ifac[i - 1] = ifac[i] * i % mod;
    }
    ll choose(int n, int k) {
        if (n < 0 || k < 0 || k > n || n >= int(fac.size())) return 0;
        return fac[n] * ifac[k] % mod * ifac[n - k] % mod;
    }
};

// 调用示例：
// Comb comb;
// comb.init(n, mod);
// ll ans = comb.choose(n, k);
