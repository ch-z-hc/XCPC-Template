/*
模幂 O(log(b + 1))，b 非负，1 <= mod <= INT_MAX。
求逆元须用质数模。连续求逆要求 n < mod，O(n)；批量求逆要求每项模 mod 非零，O(n + log mod)。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

ll pow_mod(ll a, unsigned long long b, ll mod) {
    a %= mod;
    if (a < 0) a += mod;
    ll ans = 1 % mod;
    for (; b; b >>= 1, a = a * a % mod)
        if (b & 1) ans = ans * a % mod;
    return ans;
}

V < ll > linear_inverse(int n, ll mod) {
    V < ll > inv(n + 1);
    if (n) inv[1] = 1;
    for (int i = 2; i <= n; i ++) inv[i] = mod - (mod / i) * inv[mod % i] % mod;
    return inv;
}

V < ll > batch_inverse(const V < ll > &a, ll mod) {
    int n = int(a.size());
    V < ll > pre(n + 1, 1), ans(n);
    for (int i = 0; i < n; i ++) {
        ll x = a[i] % mod;
        if (x < 0) x += mod;
        pre[i + 1] = pre[i] * x % mod;
    }
    ll now = pow_mod(pre[n], mod - 2, mod);
    for (int i = n - 1; i >= 0; i --) {
        ll x = a[i] % mod;
        if (x < 0) x += mod;
        ans[i] = now * pre[i] % mod;
        now = now * x % mod;
    }
    return ans;
}

/*
ll x = pow_mod(-2, 3, 7);
auto inv = linear_inverse(4, 7);
auto vals = batch_inverse({2, -3}, 7);
*/
