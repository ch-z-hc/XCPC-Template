/*
求 C(n, k) mod p，n、k 为非负 ll，p 为可建阶乘表的小质数。
预处理 O(p)，查询 O(log_p(n + 1))。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct Lucas {
    int p = 2;
    V < ll > fac, ifac;

    void init(int mod) {
        p = mod;
        fac.assign(p, 1);
        ifac.assign(p, 1);
        for (int i = 1; i < p; i ++) fac[i] = fac[i - 1] * i % p;
        ll a = fac[p - 1], res = 1;
        for (int b = p - 2; b; b >>= 1) {
            if (b & 1) res = res * a % p;
            a = a * a % p;
        }
        ifac[p - 1] = res;
        for (int i = p - 1; i >= 1; i --) ifac[i - 1] = ifac[i] * i % p;
    }

    ll choose(ll n, ll k) {
        if (n < 0 || k < 0 || k > n) return 0;
        ll ans = 1;
        for (; n || k; n /= p, k /= p) {
            int a = int(n % p), b = int(k % p);
            if (b > a) return 0;
            ans = ans * fac[a] % p * ifac[b] % p * ifac[a - b] % p;
        }
        return ans;
    }
};

// Lucas comb; comb.init(p); ll ans = comb.choose(n, k);
