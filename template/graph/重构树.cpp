#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 原顶点编号 1..n，新节点从 n + 1 编号。build 默认按边权递增，可换方向重建。
// 递增时 query 求所有 u 到 v 路径中「最大边权的最小值」，递减时求「最小边权的最大值」。
// query 连通时把答案写入 ans，同点取 0。两点不连通时 query 返回 false，lca 返回 0。
// 建树 O(m log m + n log n)，空间 O(m + n log n)，查询 O(log n)。长链注意递归栈。
struct KruskalTree {
    struct Edge {
        int u, v;
        ll w;
    };
    int n = 0, tot = 0, lg = 1;
    V < Edge > edges;
    V < V < int > > t, up;
    V < int > fa, dep, bel, roots;
    V < ll > weight;

    KruskalTree(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        edges.clear();
        tot = 0;
    }
    void add_edge(int u, int v, ll w) { edges.push_back({u, v, w}); }
    int find(int u) { return fa[u] == u ? u : fa[u] = find(fa[u]); }

    void dfs(int u, int p, int root) {
        up[0][u] = p; bel[u] = root;
        for (int j = 1; j < lg; ++ j) up[j][u] = up[j - 1][up[j - 1][u]];
        for (int v : t[u]) {
            dep[v] = dep[u] + 1;
            dfs(v, u, root);
        }
    }

    void build(bool rev = false) {
        tot = n;
        fa.resize(2 * n + 1); iota(fa.begin(), fa.end(), 0);
        t.assign(2 * n + 1, {}); weight.assign(2 * n + 1, 0);
        auto e = edges;
        sort(e.begin(), e.end(), [&](Edge a, Edge b) {
            return rev ? a.w > b.w : a.w < b.w;
        });
        for (auto [u, v, w] : e) {
            u = find(u); v = find(v);
            if (u == v) continue;
            ++ tot;
            t[tot] = {u, v};
            weight[tot] = w;
            fa[u] = fa[v] = tot;
        }
        roots.clear();
        for (int u = 1; u <= tot; ++ u) if (fa[u] == u) roots.push_back(u);
        lg = 1;
        while ((1ll << lg) <= max(1, tot)) lg ++;
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

// KruskalTree tree(n); tree.add_edge(u, v, w); tree.build();
// ll ans; bool ok = tree.query(u, v, ans);
