#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 先复制 NTT.cpp；第一类为无符号斯特林数，第二类为集合划分数。
// stirling_first / second 返回 [0..n][0..n]，要求 n >= 0、1 <= mod <= 2^31。
// 递推表 O(n^2) 时间、空间，模数可为合数；s(0,0) = S(0,0) = 1 % mod。
// stirling_second_row 返回 S(n,0..n)，固定模数 998244353。
// 卷积版 O(n log n) 时间、O(n) 空间，要求 0 <= n < 2^22；0^0 取 1。
V < V < ll > > stirling_first(int n, ll mod) {
    V < V < ll > > s(n + 1, V < ll > (n + 1));
    s[0][0] = 1 % mod;
    for (int i = 1; i <= n; ++ i)
        for (int j = 1; j <= i; ++ j)
            s[i][j] = (s[i - 1][j - 1] + (i - 1) * s[i - 1][j]) % mod;
    return s;
}

V < V < ll > > stirling_second(int n, ll mod) {
    V < V < ll > > s(n + 1, V < ll > (n + 1));
    s[0][0] = 1 % mod;
    for (int i = 1; i <= n; ++ i)
        for (int j = 1; j <= i; ++ j)
            s[i][j] = (s[i - 1][j - 1] + j * s[i - 1][j]) % mod;
    return s;
}

V < ll > stirling_second_row(int n) {
    V < ll > fac(n + 1, 1), ifac(n + 1, 1), a(n + 1), b(n + 1);
    for (int i = 1; i <= n; ++ i) fac[i] = fac[i - 1] * i % NTT_MOD;
    ifac[n] = ntt_pow(fac[n], NTT_MOD - 2);
    for (int i = n; i; -- i) ifac[i - 1] = ifac[i] * i % NTT_MOD;
    for (int i = 0; i <= n; ++ i) {
        a[i] = ntt_pow(i, n) * ifac[i] % NTT_MOD;
        b[i] = (i & 1) ? (NTT_MOD - ifac[i]) % NTT_MOD : ifac[i];
    }
    a = multiply(a, b); a.resize(n + 1);
    return a;
}

// auto s = stirling_first(n, mod); auto t = stirling_second(n, mod);
// auto row = stirling_second_row(n);
