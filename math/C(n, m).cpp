#include <bits/stdc++.h>
using namespace std;

// 质数模数下的组合数，要求 0 <= n < mod；表下标范围 0..n。
// 预处理 O(n)，查询 O(1)，空间 O(n)。
struct CombinationMod {
    long long mod = 1;
    vector<long long> fact, inv_fact;

    void init(int n, long long mod_) {
        mod = mod_; fact.assign(n + 1, 1); inv_fact.assign(n + 1, 1);
        for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i % mod;
        auto power = [&](long long a, long long e) {
            long long r = 1;
            for (; e; e >>= 1, a = a * a % mod) if (e & 1) r = r * a % mod;
            return r;
        };
        inv_fact[n] = power(fact[n], mod - 2);
        for (int i = n; i >= 1; --i) inv_fact[i - 1] = inv_fact[i] * i % mod;
    }
    long long choose(int n, int k) const {
        if (n < 0 || k < 0 || k > n || n >= int(fact.size())) return 0;
        return fact[n] * inv_fact[k] % mod * inv_fact[n - k] % mod;
    }
};
