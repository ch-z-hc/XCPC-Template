/*
模幂 O(log(b + 1))，b 非负，1 <= mod <= INT_MAX。
求逆元须用质数模。连续求逆要求 n < mod，O(n)；批量求逆要求每项模 mod 非零，O(n + log mod)。
*/

#include <bits/stdc++.h>

using i64 = long long;

i64 pow_mod(i64 a, unsigned long long b, i64 mod) {
    a %= mod;
    if (a < 0) a += mod;
    i64 ans = 1 % mod;
    for (; b; b >>= 1, a = a * a % mod)
        if (b & 1) ans = ans * a % mod;
    return ans;
}

std::vector<i64> linear_inverse(int n, i64 mod) {
    std::vector<i64> inv(n + 1);
    if (n) inv[1] = 1;
    for (int i = 2; i <= n; i ++) inv[i] = mod - (mod / i) * inv[mod % i] % mod;
    return inv;
}

std::vector<i64> batch_inverse(const std::vector<i64> &a, i64 mod) {
    int n = int(a.size());
    std::vector<i64> pre(n + 1, 1), ans(n);
    for (int i = 0; i < n; i ++) {
        i64 x = a[i] % mod;
        if (x < 0) x += mod;
        pre[i + 1] = pre[i] * x % mod;
    }
    i64 now = pow_mod(pre[n], mod - 2, mod);
    for (int i = n - 1; i >= 0; i --) {
        i64 x = a[i] % mod;
        if (x < 0) x += mod;
        ans[i] = now * pre[i] % mod;
        now = now * x % mod;
    }
    return ans;
}

/*
i64 x = pow_mod(-2, 3, 7);
auto inv = linear_inverse(4, 7);
auto vals = batch_inverse({2, -3}, 7);
*/
