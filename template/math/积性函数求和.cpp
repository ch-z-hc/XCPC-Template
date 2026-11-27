/*
求积性函数 f 的前缀和，f(1) = 1，f(p) 须为 c[0] + c[1]p + c[2]p²。
0 <= n <= 10^12，2 <= mod <= INT_MAX。
素数幂回调 O(1) 时，常用时间估计 O(n^(3/4) / log n)，空间 O(sqrt(n))。
pi 返回素数个数的原值，sum 返回前缀和对 mod 的余数。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct Min25 {
    ll n = 0, mod = 2;
    int lim = 0;
    V < int > prime, small, large;
    V < ll > w, cnt;
    V < array < ll, 3 > > g;

    int id(ll x) { return x <= lim ? small[x] : large[n / x]; }
    ll norm(ll x) {
        x %= mod;
        return x < 0 ? x + mod : x;
    }

    void init(ll n_, ll mod_) {
        n = n_;
        mod = mod_;
        lim = int(sqrtl(n));
        while (1ll * (lim + 1) * (lim + 1) <= n) ++ lim;
        while (1ll * lim * lim > n) -- lim;
        prime.clear(); w.clear();
        cnt.clear(); g.clear();
        small.assign(lim + 1, -1); large.assign(lim + 1, -1);
        V < bool > vis(lim + 1);
        for (int i = 2; i <= lim; i ++) {
            if (!vis[i]) prime.push_back(i);
            for (int p : prime) {
                if (1ll * i * p > lim) break;
                vis[i * p] = true;
                if (i % p == 0) break;
            }
        }
        for (ll l = 1, r; l <= n; l = r + 1) {
            ll x = n / l;
            r = n / x;
            int k = int(w.size());
            if (x <= lim) small[x] = k;
            else large[n / x] = k;
            w.push_back(x);
            cnt.push_back(x - 1);
            ll a = x, b = x + 1, c = 2 * x + 1;
            if (a % 2 == 0) a /= 2;
            else b /= 2;
            ll s1 = a % mod * (b % mod) % mod;
            if (a % 3 == 0) a /= 3;
            else if (b % 3 == 0) b /= 3;
            else c /= 3;
            ll s2 = a % mod * (b % mod) % mod * (c % mod) % mod;
            g.push_back({(x - 1) % mod, norm(s1 - 1), norm(s2 - 1)});
        }
        array < ll, 3 > pre{};
        int seen = 0;
        for (int p : prime) {
            array < ll, 3 > power{1, p % mod, 1ll * p * p % mod};
            for (int j = 0; j < int(w.size()) && 1ll * p * p <= w[j]; j ++) {
                int k = id(w[j] / p);
                cnt[j] -= cnt[k] - seen;
                for (int d = 0; d < 3; d ++)
                    g[j][d] = norm(g[j][d]
                                - power[d] * norm(g[k][d] - pre[d]) % mod);
            }
            for (int d = 0; d < 3; d ++) pre[d] = (pre[d] + power[d]) % mod;
            ++ seen;
        }
    }

    ll pi() { return n < 2 ? 0 : cnt[0]; }

    template < class F > ll sum(array < ll, 3 > c, F f) {
        if (!n) return 0;
        for (ll &x : c) x = norm(x);
        V < ll > value(w.size()), pre(prime.size() + 1);
        for (int i = 0; i < int(w.size()); i ++)
            for (int d = 0; d < 3; d ++)
                value[i] = (value[i] + c[d] * g[i][d]) % mod;
        for (int i = 0; i < int(prime.size()); i ++) {
            ll p = prime[i] % mod;
            ll val = (c[0] + c[1] * p % mod + c[2] * p % mod * p) % mod;
            pre[i + 1] = (pre[i] + val) % mod;
        }
        auto dfs = [&](auto &&self, ll x, int k) -> ll {
            if (x < 2 || (k < int(prime.size()) && prime[k] > x)) return 0;
            ll ans = norm(value[id(x)] - pre[k]);
            for (int i = k; i < int(prime.size()) && prime[i] <= x / prime[i]; i ++) {
                ll p = prime[i], cur = p;
                for (int e = 1; cur <= x / p; e ++, cur *= p) {
                    ll a = norm(f(p, e));
                    ll b = norm(f(p, e + 1));
                    ans = (ans + a * self(self, x / cur, i + 1) % mod + b) % mod;
                }
            }
            return ans;
        };
        return (1 + dfs(dfs, n, 0)) % mod;
    }
};

/*
Min25 sieve; sieve.init(100, 1000000007); ll primes = sieve.pi();
ll sum = sieve.sum({2, 0, 0}, [](ll, int e) -> ll { return e + 1; });
*/
