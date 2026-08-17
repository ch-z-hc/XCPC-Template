#include <bits/stdc++.h>
using namespace std;

// 拉格朗日插值单点求值；x、y 使用 0-based 下标且长度相同，横坐标两两不同。
// mod 必须为质数；复杂度 O(n^2)，额外空间 O(1)。
static long long interpolation_pow(long long a, long long e, long long mod) {
    long long r = 1 % mod;
    for (; e; e >>= 1, a = a * a % mod)
        if (e & 1) r = r * a % mod;
    return r;
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
