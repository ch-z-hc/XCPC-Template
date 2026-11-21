#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
det 对从 0 编号的方阵求行列式，允许负元素，空矩阵返回 1。模数须为 [2, INT_MAX] 内的质数。
matrix_tree 的顶点编号 1..n，每条边传 {u, v, w}。可有重边，自环跳过，单点图返回 1。
默认求无向生成树。传 directed = true 时求从 root 向外的树。每棵树贡献边权乘积。
耗时 O(n³ + n log mod)，建矩阵另需 O(m)，空间 O(n²)。
*/

ll det(V < V < ll > > a, ll mod) {
    assert(2 <= mod && mod <= INT_MAX);
    int n = int(a.size());
    for (auto &row : a) {
        assert(int(row.size()) == n);
        for (ll &x : row) x = (x % mod + mod) % mod;
    }
    auto power = [&](ll a, ll b) {
        ll ans = 1;
        for (; b; b >>= 1, a = a * a % mod)
            if (b & 1) ans = ans * a % mod;
        return ans;
    };
    ll ans = 1;
    for (int i = 0; i < n; ++ i) {
        int p = i;
        while (p < n && ! a[p][i]) p ++;
        if (p == n) return 0;
        if (p != i) swap(a[p], a[i]), ans = mod - ans;
        ans = ans * a[i][i] % mod;
        ll iv = power(a[i][i], mod - 2);
        for (int j = i + 1; j < n; ++ j) {
            ll v = a[j][i] * iv % mod;
            for (int k = i; k < n; ++ k)
                a[j][k] = (a[j][k] - v * a[i][k] % mod + mod) % mod;
        }
    }
    return ans;
}

ll matrix_tree(int n, const V < tuple < int, int, ll > > &edges,
               ll mod, int root = 1, bool directed = false) {
    assert(n >= 1 && 1 <= root && root <= n);
    assert(2 <= mod && mod <= INT_MAX);
    V < V < ll > > a(n - 1, V < ll > (n - 1));
    auto add = [&](int u, int v, ll w) {
        if (u == root || v == root) return;
        int i = u - 1 - (u > root), j = v - 1 - (v > root);
        a[i][j] = (a[i][j] + w + mod) % mod;
    };
    for (auto [u, v, w] : edges) {
        assert(1 <= u && u <= n && 1 <= v && v <= n);
        if (u == v) continue;
        w = (w % mod + mod) % mod;
        add(v, v, w); add(v, u, -w);
        if (! directed) add(u, u, w), add(u, v, -w);
    }
    return det(a, mod);
}

/*
det({{1, 2}, {3, 4}}, 998244353);
matrix_tree(3, {{1, 2, 1}, {2, 3, 1}, {1, 3, 1}}, 998244353);
*/
