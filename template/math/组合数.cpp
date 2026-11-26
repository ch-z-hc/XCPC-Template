// 质数模下预处理组合数，0 <= n < mod <= 2^31。建表 O(n + log mod)，查询 O(1)，超出表长返回 0。

#include <bits/stdc++.h>

using i64 = long long;

struct Comb {
    i64 mod = 1;
    std::vector<i64> fac, ifac;

    void init(int n, i64 mod_) {
        mod = mod_;
        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);
        for (int i = 1; i <= n; i ++) fac[i] = fac[i - 1] * i % mod;
        auto power = [&](i64 a, i64 b) {
            i64 res = 1;
            for (; b; b >>= 1) {
                if (b & 1) res = 1ll * res * a % mod;
                a = 1ll * a * a % mod;
            }
            return res;
        };
        ifac[n] = power(fac[n], mod - 2);
        for (int i = n; i >= 1; i --) ifac[i - 1] = ifac[i] * i % mod;
    }
    i64 choose(int n, int k) {
        if (n < 0 || k < 0 || k > n || n >= int(fac.size())) return 0;
        return fac[n] * ifac[k] % mod * ifac[n - k] % mod;
    }
};

/*
Comb comb;
comb.init(n, mod);
i64 ans = comb.choose(n, k);
*/
