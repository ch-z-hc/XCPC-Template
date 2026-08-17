#include <bits/stdc++.h>
using namespace std;

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

// 点的横坐标需要两两不同，且 mod 为质数；复杂度 O(n^2)。
