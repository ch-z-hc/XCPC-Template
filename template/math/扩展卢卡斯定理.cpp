#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 扩展 Lucas：任意正 int 模数下的 C(n, k)，不要求模数为质数。
// 0 <= n <= LLONG_MAX；k < 0 或 k > n 时返回 0，mod = 1 时返回 0。
// 将 mod 分解为互质的质数幂 p^e，每个质数幂均需预处理完整阶乘表。
// init 时间 O(sqrt(mod) + sum p^e)，空间 O(sum p^e)，必须能存下这些表。
// choose 时间 O(omega(mod) * (log^2(n + 2) + log mod))。
// 阶乘表为 0-based；内部乘法用 ll，mod <= INT_MAX 时不会溢出。

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
        for (ll i = 1; i <= q; ++ i) {
            b.fac[i] = b.fac[i - 1];
            if (i % p) b.fac[i] = b.fac[i] * i % q;
        }
    }

    void init(int m) {
        mod = m; a.clear();
        int x = m;
        for (int p = 2; 1ll * p * p <= x; ++ p) {
            if (x % p) continue;
            int q = 1;
            do { q *= p; x /= p; } while (x % p == 0);
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
        if (! n) return 1;
        ll ans = power(b.fac[b.mod], n / b.mod, b.mod);
        ans = ans * b.fac[n % b.mod] % b.mod;
        return ans * fact(n / b.p, b) % b.mod;
    }

    ll choose(ll n, ll k) {
        if (k < 0 || k > n || mod == 1) return 0;
        ll ans = 0;
        for (const auto &b : a) {
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

// 调用示例：ExLucas c; c.init(1000); ll ans = c.choose(n, k);
