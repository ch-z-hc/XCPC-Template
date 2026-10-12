#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef __int128_t ModInt;

using namespace std;

// pow_mod：mod > 0，指数非负；支持负底数和整个正 ll 模数范围。
// linear_inverse：mod 为质数，0 <= n < mod，返回 1..n 的逆元，inv[0] = 0。
// batch_inverse：mod 为质数，a 为 0-based，每项模 mod 后非零。
// 时间分别 O(log b)、O(n)、O(n + log mod)，空间 O(1)、O(n)、O(n)。
// 乘法使用 __int128，需 GCC / Clang；本文件不提供合数模数逆元。
ll pow_mod(ll a, unsigned long long b, ll mod) {
    a %= mod;
    if (a < 0) a += mod;
    ll ans = 1 % mod;
    for (; b; b >>= 1, a = ModInt(a) * a % mod)
        if (b & 1) ans = ModInt(ans) * a % mod;
    return ans;
}

V < ll > linear_inverse(int n, ll mod) {
    V < ll > inv(n + 1);
    if (n) inv[1] = 1;
    for (int i = 2; i <= n; ++ i)
        inv[i] = mod - ModInt(mod / i) * inv[mod % i] % mod;
    return inv;
}

V < ll > batch_inverse(const V < ll > &a, ll mod) {
    int n = int(a.size());
    V < ll > pre(n + 1, 1), ans(n);
    for (int i = 0; i < n; ++ i) {
        ll x = a[i] % mod;
        if (x < 0) x += mod;
        pre[i + 1] = ModInt(pre[i]) * x % mod;
    }
    ll now = pow_mod(pre[n], mod - 2, mod);
    for (int i = n - 1; i >= 0; -- i) {
        ll x = a[i] % mod;
        if (x < 0) x += mod;
        ans[i] = ModInt(now) * pre[i] % mod;
        now = ModInt(now) * x % mod;
    }
    return ans;
}

// 调用：ll x = pow_mod(a, b, mod);
// auto inv = linear_inverse(n, mod); auto values = batch_inverse(a, mod);
