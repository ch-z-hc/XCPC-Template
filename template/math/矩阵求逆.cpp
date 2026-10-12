#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 质数模数 2 <= mod <= INT_MAX；输入为 0-based 方阵，允许负元素。
// 成功返回 true，逆矩阵写入 b；不可逆返回 false，此时不要使用 b。
// 空矩阵可逆，输出空矩阵。
// Gauss-Jordan 消元，时间 O(n^3 + n log mod)，额外空间 O(n^2)。
bool matrix_inv(V < V < ll > > a, int mod, V < V < ll > > &b) {
    int n = int(a.size());
    b.assign(n, V < ll > (n));
    for (int i = 0; i < n; ++ i) {
        b[i][i] = 1;
        for (ll &x : a[i]) { x %= mod; if (x < 0) x += mod; }
    }
    auto power = [&](ll x, int k) {
        ll ans = 1;
        for (; k; k >>= 1, x = x * x % mod)
            if (k & 1) ans = ans * x % mod;
        return ans;
    };
    for (int i = 0; i < n; ++ i) {
        int p = i;
        while (p < n && ! a[p][i]) ++ p;
        if (p == n) return false;
        swap(a[i], a[p]); swap(b[i], b[p]);
        ll inv = power(a[i][i], mod - 2);
        for (int j = 0; j < n; ++ j) {
            a[i][j] = a[i][j] * inv % mod;
            b[i][j] = b[i][j] * inv % mod;
        }
        for (int u = 0; u < n; ++ u) if (u != i) {
            ll k = a[u][i];
            for (int j = 0; j < n; ++ j) {
                a[u][j] = (a[u][j] - k * a[i][j] % mod + mod) % mod;
                b[u][j] = (b[u][j] - k * b[i][j] % mod + mod) % mod;
            }
        }
    }
    return true;
}

// 调用：V < V < ll > > b; bool ok = matrix_inv(a, 1000000007, b);
