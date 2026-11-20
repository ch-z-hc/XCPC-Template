#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 给 n 对采样值 x[i]、y[i]，求插值多项式在 at 处的值。两个数组从 0 数，长度相同。
// mod 须为质数且 <= 2^31，各坐标先化到 [0, mod)，横坐标模 mod 两两不同。
// 逐点求逆，时间 O(n² + n log mod)，额外空间 O(1)。

static ll lagrange_pow(ll a, ll b, ll mod) {
    ll res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

ll lagrange(const V < ll > &x, const V < ll > &y, ll at, ll mod) {
    int n = int(x.size());
    ll ans = 0;
    for (int i = 0; i < n; ++ i) {
        ll num = 1, den = 1;
        for (int j = 0; j < n; ++ j) if (i != j) {
            num = num * ((at - x[j]) % mod + mod) % mod;
            den = den * ((x[i] - x[j]) % mod + mod) % mod;
        }
        ans = (ans + y[i] * num % mod *
                  lagrange_pow(den, mod - 2, mod)) % mod;
    }
    return ans;
}

// ll ans = lagrange(x, y, at, mod);
