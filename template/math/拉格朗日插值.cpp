/*
给定 n 个采样点，计算插值多项式在 at 处的值，O(n² + n log mod)。
两数组等长，坐标先取模，横坐标在模意义下两两不同；mod 为不超过 2^31 的质数。
*/

#include <bits/stdc++.h>

using i64 = long long;

static i64 lagrange_pow(i64 a, i64 b, i64 mod) {
    i64 res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

i64 lagrange(const std::vector<i64> &x, const std::vector<i64> &y, i64 at, i64 mod) {
    int n = int(x.size());
    i64 ans = 0;
    for (int i = 0; i < n; i ++) {
        i64 num = 1, den = 1;
        for (int j = 0; j < n; j ++)
            if (i != j) {
                num = num * ((at - x[j]) % mod + mod) % mod;
                den = den * ((x[i] - x[j]) % mod + mod) % mod;
            }
        ans = (ans + y[i] * num % mod * lagrange_pow(den, mod - 2, mod)) % mod;
    }
    return ans;
}

// i64 ans = lagrange(x, y, at, mod);
