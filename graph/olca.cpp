#include <bits/stdc++.h>
using namespace std;

// Euler Tour + RMQ，预处理 O(n log n)，查询 O(1)，下标 1-based。
struct O1LCA {
    int n = 0;
    vector<vector<int>> graph;
    vector<int> first, depth, euler, lg;
    vector<vector<int>> st;

    explicit O1LCA(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        graph.assign(n + 1, {});
        first.assign(n + 1, -1);
        depth.assign(n + 1, 0);
        euler.clear();
        lg.clear();
        st.clear();
    }

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    void dfs(int u, int parent, int dep) {
        depth[u] = dep;
        first[u] = int(euler.size());
        euler.push_back(u);
        for (int v : graph[u]) {
            if (v == parent) continue;
            dfs(v, u, dep + 1);
            euler.push_back(u);
        }
    }

    void build(int root = 1) {
        euler.clear();
        fill(first.begin(), first.end(), -1);
        dfs(root, 0, 0);
        int m = int(euler.size());
        lg.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) lg[i] = lg[i >> 1] + 1;
        st.assign(lg[m] + 1, vector<int>(m));
        iota(st[0].begin(), st[0].end(), 0);
        for (int j = 1; j <= lg[m]; ++j)
            for (int i = 0; i + (1 << j) <= m; ++i) {
                int x = st[j - 1][i];
                int y = st[j - 1][i + (1 << (j - 1))];
                st[j][i] = depth[euler[x]] < depth[euler[y]] ? x : y;
            }
    }

    int lca(int u, int v) const {
        int l = first[u], r = first[v];
        if (l > r) swap(l, r);
        int k = lg[r - l + 1];
        int x = st[k][l], y = st[k][r - (1 << k) + 1];
        return depth[euler[x]] < depth[euler[y]] ? euler[x] : euler[y];
    }
};

// Usage:
// O1LCA lca(n);
// lca.add_edge(u, v);
// lca.build(root);
// int p = lca.lca(u, v);
