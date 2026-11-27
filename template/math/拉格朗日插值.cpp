/*
给定 n 个采样点，计算插值多项式在 at 处的值，O(n² + n log mod)。
两数组等长，坐标先取模，横坐标在模意义下两两不同；mod 为不超过 2^31 的质数。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

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
    for (int i = 0; i < n; i ++) {
        ll num = 1, den = 1;
        for (int j = 0; j < n; j ++)
            if (i != j) {
                num = num * ((at - x[j]) % mod + mod) % mod;
                den = den * ((x[i] - x[j]) % mod + mod) % mod;
            }
        ans = (ans + y[i] * num % mod * lagrange_pow(den, mod - 2, mod)) % mod;
    }
    return ans;
}

// ll ans = lagrange(x, y, at, mod);
