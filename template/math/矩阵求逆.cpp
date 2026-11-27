// 质数模方阵求逆，O(n³ + n log mod)。元素可负，模数在 [2, INT_MAX] 内。奇异矩阵返回 false。

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

bool matrix_inv(V < V < ll > > a, int mod, V < V < ll > > &b) {
    int n = int(a.size());
    b.assign(n, V < ll >(n));
    for (int i = 0; i < n; i ++) {
        b[i][i] = 1;
        for (ll &x : a[i]) {
            x %= mod;
            if (x < 0) x += mod;
        }
    }
    auto power = [&](ll x, int k) {
        ll ans = 1;
        for (; k; k >>= 1, x = x * x % mod)
            if (k & 1) ans = ans * x % mod;
        return ans;
    };
    for (int i = 0; i < n; i ++) {
        int p = i;
        while (p < n && !a[p][i]) ++ p;
        if (p == n) return false;
        swap(a[i], a[p]); swap(b[i], b[p]);
        ll inv = power(a[i][i], mod - 2);
        for (int j = 0; j < n; j ++) {
            a[i][j] = a[i][j] * inv % mod; b[i][j] = b[i][j] * inv % mod;
        }
        for (int u = 0; u < n; u ++)
            if (u != i) {
                ll k = a[u][i];
                for (int j = 0; j < n; j ++) {
                    a[u][j] = (a[u][j] - k * a[i][j] % mod + mod) % mod;
                    b[u][j] = (b[u][j] - k * b[i][j] % mod + mod) % mod;
                }
            }
    }
    return true;
}

// V < V < ll > > b; bool ok = matrix_inv(a, 1000000007, b);
