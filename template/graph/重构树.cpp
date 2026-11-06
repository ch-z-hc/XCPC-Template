#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// Kruskal 重构森林：原顶点 1..n，合并节点从 n + 1 开始，权值可为负。
// build 默认按边权递增，query 求路径最大边权的最小值；递减时反之。
// query(u, v, ans) 连通返回 true 并写入 ans，不连通返回 false。
// 同点约定 ans = 0；lca 不连通返回 0。
// build 时间 O(m log m + n log n)，空间 O(m + n log n)，query 为 O(log n)。
// 可重复 build / 更换排序方向；t 为有根森林子节点，递归深度最坏 O(n)。
struct KruskalTree {
    struct Edge { int u, v; ll w; };
    int n = 0, tot = 0, lg = 1;
    V < Edge > edges;
    V < V < int > > t, up;
    V < int > fa, dep, bel, roots;
    V < ll > weight;

    KruskalTree(int n = 0) { init(n); }
    void init(int n_) { n = n_; edges.clear(); tot = 0; }
    void add_edge(int u, int v, ll w) { edges.push_back({u, v, w}); }
    int find(int u) { return fa[u] == u ? u : fa[u] = find(fa[u]); }

    void dfs(int u, int p, int root) {
        up[0][u] = p; bel[u] = root;
        for (int j = 1; j < lg; ++ j) up[j][u] = up[j - 1][up[j - 1][u]];
        for (int v : t[u]) { dep[v] = dep[u] + 1; dfs(v, u, root); }
    }

    void build(bool descending = false) {
        tot = n;
        fa.resize(2 * n + 1); iota(fa.begin(), fa.end(), 0);
        t.assign(2 * n + 1, {}); weight.assign(2 * n + 1, 0);
        auto e = edges;
        sort(e.begin(), e.end(), [&](Edge a, Edge b) {
            return descending ? a.w > b.w : a.w < b.w;
        });
        for (auto [u, v, w] : e) {
            u = find(u); v = find(v);
            if (u == v) continue;
            ++ tot; t[tot] = {u, v}; weight[tot] = w;
            fa[u] = fa[v] = tot;
        }
        roots.clear();
        for (int u = 1; u <= tot; ++ u) if (fa[u] == u) roots.push_back(u);
        lg = 1; while ((1ll << lg) <= max(1, tot)) lg ++;
        up.assign(lg, V < int > (tot + 1));
        dep.assign(tot + 1, 0); bel.assign(tot + 1, 0);
        for (int root : roots) dfs(root, 0, root);
    }

    int lca(int u, int v) {
        if (bel[u] != bel[v]) return 0;
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int j = 0; j < lg; ++ j) if (d >> j & 1) u = up[j][u];
        if (u == v) return u;
        for (int j = lg - 1; j >= 0; -- j)
            if (up[j][u] != up[j][v]) { u = up[j][u]; v = up[j][v]; }
        return up[0][u];
    }

    bool query(int u, int v, ll &ans) {
        int p = lca(u, v);
        if (! p) return false;
        ans = u == v ? 0 : weight[p];
        return true;
    }
};

// 用法：KruskalTree tree(n); tree.add_edge(u, v, w); tree.build();
// ll ans; bool ok = tree.query(u, v, ans);
// tree.t、tree.weight 可用于额外的树上统计。
