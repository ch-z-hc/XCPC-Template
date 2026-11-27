/*
求 C(n, k) 对任意正 int 模数的余数，n 为非负 ll。k 越界或模数为 1 时返回 0。
为 mod 的每个质数幂 q 建整表，建表 O(sqrt(mod) + Σq)，空间 O(Σq)，大质数模尤其占内存。
设不同质因子有 t 个，查询 O(t (log²(n + 2) + log mod))。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

struct ExLucas {
    struct Factor {
        int p, mod;
        V < ll > fac;
    };
    int mod;
    V < Factor > a;

    ll power(ll x, ll n, int m) {
        ll ans = 1;
        for (; n; n >>= 1, x = x * x % m)
            if (n & 1) ans = ans * x % m;
        return ans;
    }

    void add(int p, int q) {
        a.push_back({p, q, {}});
        Factor &b = a.back();
        b.fac.assign(size_t(q) + 1, 1);
        for (ll i = 1; i <= q; i ++) {
            b.fac[i] = b.fac[i - 1];
            if (i % p) b.fac[i] = b.fac[i] * i % q;
        }
    }

    void init(int m) {
        mod = m;
        a.clear();
        int x = m;
        for (int p = 2; 1ll * p * p <= x; p ++) {
            if (x % p) continue;
            int q = 1;
            do {
                q *= p;
                x /= p;
            } while (x % p == 0);
            add(p, q);
        }
        if (x > 1) add(x, x);
    }

    ll count(ll n, int p) {
        ll ans = 0;
        for (; n; n /= p) ans += n / p;
        return ans;
    }

    ll fact(ll n, const Factor &b) {
        if (!n) return 1;
        ll ans = power(b.fac[b.mod], n / b.mod, b.mod);
        ans = ans * b.fac[n % b.mod] % b.mod;
        return ans * fact(n / b.p, b) % b.mod;
    }

    ll choose(ll n, ll k) {
        if (k < 0 || k > n || mod == 1) return 0;
        ll ans = 0;
        for (auto &b : a) {
            int q = b.mod, phi = q - q / b.p;
            ll e = count(n, b.p) - count(k, b.p) - count(n - k, b.p);
            ll den = fact(k, b) * fact(n - k, b) % q;
            ll r = fact(n, b) * power(den, phi - 1, q) % q;
            r = r * power(b.p, e, q) % q;
            ll m = mod / q;
            ans = (ans + r * m % mod * power(m % q, phi - 1, q)) % mod;
        }
        return ans;
    }
};

// ExLucas c; c.init(1000); ll ans = c.choose(n, k);
