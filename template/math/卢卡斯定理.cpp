/*
求 C(n, k) mod p，n、k 为非负 i64，p 为可建阶乘表的小质数。
预处理 O(p)，查询 O(log_p(n + 1))。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct Lucas {
    int p = 2;
    std::vector<i64> fac, ifac;

    void init(int mod) {
        p = mod;
        fac.assign(p, 1);
        ifac.assign(p, 1);
        for (int i = 1; i < p; i ++) fac[i] = fac[i - 1] * i % p;
        i64 a = fac[p - 1], res = 1;
        for (int b = p - 2; b; b >>= 1) {
            if (b & 1) res = res * a % p;
            a = a * a % p;
        }
        ifac[p - 1] = res;
        for (int i = p - 1; i >= 1; i --) ifac[i - 1] = ifac[i] * i % p;
    }

    i64 choose(i64 n, i64 k) {
        if (n < 0 || k < 0 || k > n) return 0;
        i64 ans = 1;
        for (; n || k; n /= p, k /= p) {
            int a = int(n % p), b = int(k % p);
            if (b > a) return 0;
            ans = ans * fac[a] % p * ifac[b] % p * ifac[a - b] % p;
        }
        return ans;
    }
};

// Lucas comb; comb.init(p); i64 ans = comb.choose(n, k);
