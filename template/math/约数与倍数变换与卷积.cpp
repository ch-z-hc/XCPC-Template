#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 输入用 a[1..n]，a[0] 填 0，系数先化到 [0, mod)，1 <= mod <= 2^31，可为合数。
// divisor_zeta 把 a[x] 改为 x 所有约数的值之和；multiple_zeta 对 x 的倍数求和。传 true 做逆变换。
// gcd_conv、lcm_conv 要求两数组等长，按 gcd 或 lcm 累加乘积，只保留下标 <= n 的项。
// 每次 O(n log n)。变换原地进行，卷积另用 O(n) 空间。
void divisor_zeta(V < ll > &a, ll mod, bool inverse = false) {
    int n = int(a.size()) - 1;
    for (int k = 1; k <= n; ++ k) {
        int i = inverse ? k : n - k + 1;
        for (int j = i + i; j <= n; j += i) {
            a[j] = (a[j] + (inverse ? -a[i] : a[i])) % mod;
            if (a[j] < 0) a[j] += mod;
        }
    }
}

void multiple_zeta(V < ll > &a, ll mod, bool inverse = false) {
    int n = int(a.size()) - 1;
    for (int k = 1; k <= n; ++ k) {
        int i = inverse ? n - k + 1 : k;
        for (int j = i + i; j <= n; j += i) {
            a[i] = (a[i] + (inverse ? -a[j] : a[j])) % mod;
            if (a[i] < 0) a[i] += mod;
        }
    }
}

V < ll > gcd_conv(V < ll > a, V < ll > b, ll mod) {
    multiple_zeta(a, mod); multiple_zeta(b, mod);
    for (int i = 1; i < int(a.size()); ++ i) a[i] = a[i] * b[i] % mod;
    multiple_zeta(a, mod, true);
    return a;
}

V < ll > lcm_conv(V < ll > a, V < ll > b, ll mod) {
    divisor_zeta(a, mod); divisor_zeta(b, mod);
    for (int i = 1; i < int(a.size()); ++ i) a[i] = a[i] * b[i] % mod;
    divisor_zeta(a, mod, true);
    return a;
}

// divisor_zeta(a, mod); divisor_zeta(a, mod, true); // 恢复 a。
// auto c = gcd_conv(a, b, mod); auto d = lcm_conv(a, b, mod);
