#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 拉格朗日插值单点求值；x、y 使用 0-based 下标且长度相同，横坐标两两不同。
// mod 必须为质数；复杂度 O(n^2)，额外空间 O(1)。
static ll interpolation_pow(ll a, ll b, ll mod) {
    ll res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

long long interpolation_eval(const vector<long long> &x,
                             const vector<long long> &y,
                             long long at, long long mod) {
    int n = int(x.size());
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        long long numerator = 1, denominator = 1;
        for (int j = 0; j < n; ++j) if (i != j) {
            numerator = numerator * ((at - x[j]) % mod + mod) % mod;
            denominator = denominator * ((x[i] - x[j]) % mod + mod) % mod;
        }
        answer = (answer + y[i] * numerator % mod *
                  interpolation_pow(denominator, mod - 2, mod)) % mod;
    }
    return answer;
}
