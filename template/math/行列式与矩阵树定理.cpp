/*
质数模下求行列式、生成树边权乘积之和。模数在 [2, INT_MAX] 内，允许负权、重边，忽略自环。
行列式 O(n³ + n log mod)，建矩阵另需 O(m)。空行列式和单点图均取 1。
directed 为 true 时统计从 root 向外的有向生成树。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

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
ll mod = 17, d = det({{1, 2}, {3, 4}}, mod);
V < tuple < int, int, ll > > edges{{1, 2, 1}, {2, 3, 1}, {1, 3, 1}};
ll undirected = matrix_tree(3, edges, mod);
ll directed = matrix_tree(3, edges, mod, 1, true);
*/
