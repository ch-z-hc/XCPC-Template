/*
约数和、倍数和变换及 GCD、LCM 卷积，均为 O(n log n)。a[1..n] 已取模，a[0] 填 0。
1 <= mod <= 2^31，可为合数；卷积两数组等长，只保留下标 <= n 的项。
*/

#include <bits/stdc++.h>

using i64 = long long;

void divisor_zeta(std::vector<i64> &a, i64 mod, bool inverse = false) {
    int n = int(a.size()) - 1;
    for (int k = 1; k <= n; k ++) {
        int i = inverse ? k : n - k + 1;
        for (int j = i + i; j <= n; j += i) {
            a[j] = (a[j] + (inverse ? -a[i] : a[i])) % mod;
            if (a[j] < 0) a[j] += mod;
        }
    }
}

void multiple_zeta(std::vector<i64> &a, i64 mod, bool inverse = false) {
    int n = int(a.size()) - 1;
    for (int k = 1; k <= n; k ++) {
        int i = inverse ? n - k + 1 : k;
        for (int j = i + i; j <= n; j += i) {
            a[i] = (a[i] + (inverse ? -a[j] : a[j])) % mod;
            if (a[i] < 0) a[i] += mod;
        }
    }
}

std::vector<i64> gcd_conv(std::vector<i64> a, std::vector<i64> b, i64 mod) {
    multiple_zeta(a, mod);
    multiple_zeta(b, mod);
    for (int i = 1; i < int(a.size()); i ++) a[i] = a[i] * b[i] % mod;
    multiple_zeta(a, mod, true);
    return a;
}

std::vector<i64> lcm_conv(std::vector<i64> a, std::vector<i64> b, i64 mod) {
    divisor_zeta(a, mod);
    divisor_zeta(b, mod);
    for (int i = 1; i < int(a.size()); i ++) a[i] = a[i] * b[i] % mod;
    divisor_zeta(a, mod, true);
    return a;
}

/*
std::vector<i64> a{0, 2, 4, 6}, b{0, 1, 2, 3}; i64 mod = 17;
divisor_zeta(a, mod); divisor_zeta(a, mod, true);
multiple_zeta(a, mod); multiple_zeta(a, mod, true);
auto c = gcd_conv(a, b, mod), d = lcm_conv(a, b, mod);
*/
