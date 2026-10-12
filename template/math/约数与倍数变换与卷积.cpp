#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 约数 / 倍数 zeta 与 Mobius 变换；数组下标 1..n，a[0] 必须为 0。
// divisor_zeta: b[x] = sum_{d|x} a[d]；multiple_zeta: b[x] = sum_{x|d} a[d]。
// inverse 为真时执行对应逆变换，原地操作；1 <= mod <= 2^31，可为合数。
// 输入先归一化到 [0, mod)；单次 O(n log n) 时间、O(1) 额外空间。
// 两种卷积输入长度相同，返回 n + 1 项，空间 O(n)。
// gcd_convolution 按 gcd(i,j) 聚合；lcm_convolution 仅返回 lcm(i,j) <= n 的项。
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

V < ll > gcd_convolution(V < ll > a, V < ll > b, ll mod) {
    multiple_zeta(a, mod); multiple_zeta(b, mod);
    for (int i = 1; i < int(a.size()); ++ i) a[i] = a[i] * b[i] % mod;
    multiple_zeta(a, mod, true);
    return a;
}

V < ll > lcm_convolution(V < ll > a, V < ll > b, ll mod) {
    divisor_zeta(a, mod); divisor_zeta(b, mod);
    for (int i = 1; i < int(a.size()); ++ i) a[i] = a[i] * b[i] % mod;
    divisor_zeta(a, mod, true);
    return a;
}

// divisor_zeta(a, mod); divisor_zeta(a, mod, true); // 恢复 a。
// auto c = gcd_convolution(a, b, mod); auto d = lcm_convolution(a, b, mod);
