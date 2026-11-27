/*
先复制「快速沃尔什变换」。给定非空子集权 g，令 f[0] = 1，按大小递推
f[S] = update(S, Σ g[T] f[S 去掉 T])，求和只含 S 的非空子集 T。
g 的长度为 2^n，系数和回调结果已按正 int 模数取模。时间 O(n² 2^n)，空间 O(n 2^n)。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

template < class F >
V < ll > subset_recurrence(const V < ll > &g, int mod, F update) {
    int m = int(g.size()), n = __builtin_ctz(unsigned(m));
    V < int > rank(m);
    V < V < ll > > a(n + 1, V < ll >(m)), b = a;
    V < ll > f(m);
    f[0] = 1 % mod;
    fill(a[0].begin(), a[0].end(), f[0]);
    for (int s = 1; s < m; s ++) {
        rank[s] = rank[s >> 1] + (s & 1);
        b[rank[s]][s] = g[s];
    }
    for (int k = 1; k <= n; k ++) {
        fwt_or(b[k], mod);
        for (int j = 1; j <= k; j ++)
            for (int s = 0; s < m; s ++)
                a[k][s] = (a[k][s] + b[j][s] * a[k - j][s]) % mod;
        ifwt_or(a[k], mod);
        for (int s = 0; s < m; s ++) {
            if (rank[s] == k) f[s] = a[k][s] = update(s, a[k][s]);
            else a[k][s] = 0;
        }
        fwt_or(a[k], mod);
    }
    return f;
}

/*
auto f = subset_recurrence({0, 1, 1, 0}, 998244353, [](int, ll x) { return x; });
*/
