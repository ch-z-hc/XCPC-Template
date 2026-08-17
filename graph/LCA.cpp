#include <bits/stdc++.h>
using namespace std;

// 倍增 LCA；预处理 O(n log n)，查询 O(log n)，顶点编号 1-based。
struct BinaryLCA {
    int n = 0, levels = 0;
    vector<vector<int>> graph, up;
    vector<int> depth;

    explicit BinaryLCA(int n = 0) { init(n); }
    void init(int n_) {
        n = n_; levels = 1;
        while ((1 << levels) <= max(1, n)) ++levels;
        graph.assign(n + 1, {}); up.assign(levels, vector<int>(n + 1));
        depth.assign(n + 1, 0);
    }
    void add_edge(int u, int v) { graph[u].push_back(v); graph[v].push_back(u); }
    void build(int root = 1) {
        vector<int> order{root}; up[0][root] = 0; depth[root] = 0;
        for (int i = 0; i < int(order.size()); ++i) {
            int u = order[i];
            for (int v : graph[u]) if (v != up[0][u]) {
                up[0][v] = u; depth[v] = depth[u] + 1; order.push_back(v);
            }
        }
        for (int k = 1; k < levels; ++k)
            for (int u = 1; u <= n; ++u) up[k][u] = up[k - 1][up[k - 1][u]];
    }
    int lca(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int k = 0; k < levels; ++k) if (diff >> k & 1) u = up[k][u];
        if (u == v) return u;
        for (int k = levels - 1; k >= 0; --k)
            if (up[k][u] != up[k][v]) u = up[k][u], v = up[k][v];
        return up[0][u];
    }
};
