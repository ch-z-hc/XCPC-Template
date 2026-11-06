#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 所有函数要求 1 <= mod <= INT_MAX，保证两个模值相乘不溢出 ll。
// pow_mod：指数非负，支持负底数；逆元函数另要求 mod 为质数。
// linear_inverse：mod 为质数，0 <= n < mod，返回 1..n 的逆元，inv[0] = 0。
// batch_inverse：mod 为质数，a 为 0-based，每项模 mod 后非零。
// 时间分别 O(log b)、O(n)、O(n + log mod)，空间 O(1)、O(n)、O(n)。
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
    for (int i = 2; i <= n; ++ i)
        inv[i] = mod - (mod / i) * inv[mod % i] % mod;
    return inv;
}

V < ll > batch_inverse(const V < ll > &a, ll mod) {
    int n = int(a.size());
    V < ll > pre(n + 1, 1), ans(n);
    for (int i = 0; i < n; ++ i) {
        ll x = a[i] % mod;
        if (x < 0) x += mod;
        pre[i + 1] = pre[i] * x % mod;
    }
    ll now = pow_mod(pre[n], mod - 2, mod);
    for (int i = n - 1; i >= 0; -- i) {
        ll x = a[i] % mod;
        if (x < 0) x += mod;
        ans[i] = now * pre[i] % mod;
        now = now * x % mod;
    }
    return ans;
}

// 用法：ll x = pow_mod(a, b, mod);
// auto inv = linear_inverse(n, mod); auto values = batch_inverse(a, mod);
