#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
先复制「快速数论变换与多项式乘法」。stirling1、stirling2 分别求无符号第一类、第二类斯特林数。
两者返回 0..n 的整张表，n >= 0，1 <= mod <= 2^31，可用合数模。时间和空间 O(n²)。
只求第二类第 n 行可用 stirling2_row，固定模 998244353，要求 n < 2^22。耗时 O(n log n)，空间 O(n)。
*/
V < V < ll > > stirling1(int n, ll mod) {
    V < V < ll > > s(n + 1, V < ll > (n + 1));
    s[0][0] = 1 % mod;
    for (int i = 1; i <= n; ++ i)
        for (int j = 1; j <= i; ++ j)
            s[i][j] = (s[i - 1][j - 1] + (i - 1) * s[i - 1][j]) % mod;
    return s;
}

V < V < ll > > stirling2(int n, ll mod) {
    V < V < ll > > s(n + 1, V < ll > (n + 1));
    s[0][0] = 1 % mod;
    for (int i = 1; i <= n; ++ i)
        for (int j = 1; j <= i; ++ j)
            s[i][j] = (s[i - 1][j - 1] + j * s[i - 1][j]) % mod;
    return s;
}

V < ll > stirling2_row(int n) {
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

/*
auto s = stirling1(n, mod); auto t = stirling2(n, mod);
auto row = stirling2_row(n);
*/
