/*
质数模下求行列式、生成树边权乘积之和。模数在 [2, INT_MAX] 内，允许负权、重边，忽略自环。
行列式 O(n³ + n log mod)，建矩阵另需 O(m)。空行列式和单点图均取 1。
directed 为 true 时统计从 root 向外的有向生成树。
*/

#include <bits/stdc++.h>

using i64 = long long;

i64 det(std::vector<std::vector<i64>> a, i64 mod) {
    assert(2 <= mod && mod <= INT_MAX);
    int n = int(a.size());
    for (auto &row : a) {
        assert(int(row.size()) == n);
        for (i64 &x : row) x = (x % mod + mod) % mod;
    }
    auto power = [&](i64 a, i64 b) {
        i64 ans = 1;
        for (; b; b >>= 1, a = a * a % mod)
            if (b & 1) ans = ans * a % mod;
        return ans;
    };
    i64 ans = 1;
    for (int i = 0; i < n; i ++) {
        int p = i;
        while (p < n && !a[p][i]) p ++;
        if (p == n) return 0;
        if (p != i) std::swap(a[p], a[i]), ans = mod - ans;
        ans = ans * a[i][i] % mod;
        i64 iv = power(a[i][i], mod - 2);
        for (int j = i + 1; j < n; j ++) {
            i64 v = a[j][i] * iv % mod;
            for (int k = i; k < n; k ++)
                a[j][k] = (a[j][k] - v * a[i][k] % mod + mod) % mod;
        }
    }
    return ans;
}

i64 matrix_tree(int n, const std::vector<std::tuple<int, int, i64>> &edges, i64 mod,
                int root = 1, bool directed = false) {
    assert(n >= 1 && 1 <= root && root <= n);
    assert(2 <= mod && mod <= INT_MAX);
    std::vector<std::vector<i64>> a(n - 1, std::vector<i64>(n - 1));
    auto add = [&](int u, int v, i64 w) {
        if (u == root || v == root) return;
        int i = u - 1 - (u > root), j = v - 1 - (v > root);
        a[i][j] = (a[i][j] + w + mod) % mod;
    };
    for (auto [u, v, w] : edges) {
        assert(1 <= u && u <= n && 1 <= v && v <= n);
        if (u == v) continue;
        w = (w % mod + mod) % mod;
        add(v, v, w);
        add(v, u, -w);
        if (!directed) add(u, u, w), add(u, v, -w);
    }
    return det(a, mod);
}

/*
i64 mod = 17, d = det({{1, 2}, {3, 4}}, mod);
std::vector<std::tuple<int, int, i64>> edges{{1, 2, 1}, {2, 3, 1}, {1, 3, 1}};
i64 undirected = matrix_tree(3, edges, mod);
i64 directed = matrix_tree(3, edges, mod, 1, true);
*/
