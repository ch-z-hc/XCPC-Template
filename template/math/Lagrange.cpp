#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 拉格朗日插值单点求值；x、y 使用 0-based 下标且长度相同，横坐标在 mod 意义下两两不同。
// mod 必须为质数；复杂度 O(n^2)，额外空间 O(1)。
// 要求 1 < mod <= 2^31，输入模数值归一化到 [0, mod)，乘加计算不得溢出 ll。

static ll lagrange_pow(ll a, ll b, ll mod) {
    ll res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

ll lagrange(const V < ll > &x,
                      const V < ll > &y,
                      ll at, ll mod) {
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

// 调用示例：
// ll ans = lagrange(x, y, at, mod);
