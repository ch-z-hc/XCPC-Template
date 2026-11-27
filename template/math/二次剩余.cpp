// 求 x² 与 a 模 p 同余的较小非负根，无解返回 -1。p 须为 [2, INT_MAX] 内的质数，期望 O(log p)。

#include <bits/stdc++.h>

#define fi first
#define se second

typedef long long ll;
using namespace std;

ll sqrt_mod(ll a, ll p) {
    a %= p;
    if (a < 0) a += p;
    if (!a || p == 2) return a;
    auto qpow = [&](ll x, ll k) {
        ll ans = 1;
        for (; k; k >>= 1, x = x * x % p)
            if (k & 1) ans = ans * x % p;
        return ans;
    };
    if (qpow(a, (p - 1) / 2) != 1) return -1;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution < ll > pick(0, p - 1);
    ll b, w;
    do {
        b = pick(rng);
        w = (b * b % p - a + p) % p;
    } while (qpow(w, (p - 1) / 2) != p - 1);

    auto mul = [&](pair < ll, ll > u, pair < ll, ll > v) {
        return pair < ll, ll >{
            (u.fi * v.fi + u.se * v.se % p * w) % p,
            (u.fi * v.se + u.se * v.fi) % p
        };
    };
    pair < ll, ll > ans{1, 0}, x{b, 1};
    for (ll k = (p + 1) / 2; k; k >>= 1, x = mul(x, x))
        if (k & 1) ans = mul(ans, x);
    return min(ans.fi, p - ans.fi);
}

// ll x = sqrt_mod(10, 13);
