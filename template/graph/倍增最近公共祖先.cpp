#include <bits/stdc++.h>

#define V vector

using namespace std;

// 倍增 LCA；要求输入为连通树，顶点编号 1-based。
// build 为 O(n log n)，lca 为 O(log n)，空间 O(n log n)。
// build 可更换根重复调用；dep[root] 为 0；递归深度最坏 O(n)。
struct LCA {
    int n = 0, lg = 1;
    V < V < int > > g, fa;
    V < int > dep;

    LCA(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        lg = 1;
        while ((1ll << lg) <= max(1, n)) lg ++;
        g.assign(n + 1, {});
        fa.assign(lg, V < int > (n + 1));
        dep.assign(n + 1, 0);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        fa[0][u] = p;
        for (int j = 1; j < lg; ++ j)
            fa[j][u] = fa[j - 1][fa[j - 1][u]];
        for (int v : g[u]) if (v != p) {
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
    }

    void build(int root = 1) {
        dep[root] = 0;
        dfs(root, 0);
    }

    int lca(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int j = 0; j < lg; ++ j)
            if (d >> j & 1) u = fa[j][u];
        if (u == v) return u;
        for (int j = lg - 1; j >= 0; -- j)
            if (fa[j][u] != fa[j][v]) { u = fa[j][u]; v = fa[j][v]; }
        return fa[0][u];
    }
};

// 用法：LCA tree(n);
// tree.add_edge(u, v);
// tree.build(root);
// int p = tree.lca(u, v);
