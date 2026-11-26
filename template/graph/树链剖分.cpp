/*
把树上路径拆为 O(log n) 个闭区间，子树对应 [in[u], out[u]]。建树 O(n)，求 LCA 为 O(log n)。
路径含 LCA，维护边权时跳过其位置。回调顺序不保证沿路径方向，长链注意递归栈。
文件尾的路径点权和示例需先复制「线段树与树上二分」，按 DFS 序建线段树。
*/

#include <bits/stdc++.h>

struct HLD {
    int n = 0, tim = 0;
    std::vector<std::vector<int>> g;
    std::vector<int> fa, dep, siz, son, top, in, out, rev;

    HLD(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        tim = 0;
        g.assign(n + 1, {});
        fa.assign(n + 1, 0);
        dep.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        top.assign(n + 1, 0);
        in.assign(n + 1, 0);
        out.assign(n + 1, 0);
        rev.assign(n + 1, 0);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int p) {
        fa[u] = p;
        siz[u] = 1;
        son[u] = 0;
        for (int v : g[u])
            if (v != p) {
                dep[v] = dep[u] + 1;
                dfs1(v, u);
                siz[u] += siz[v];
                if (siz[v] > siz[son[u]]) son[u] = v;
            }
    }

    void dfs2(int u, int t) {
        top[u] = t;
        in[u] = ++ tim;
        rev[tim] = u;
        if (son[u]) dfs2(son[u], t);
        for (int v : g[u])
            if (v != fa[u] && v != son[u]) dfs2(v, v);
        out[u] = tim;
    }

    void build(int root = 1) {
        tim = 0;
        dep[root] = 0;
        dfs1(root, 0);
        dfs2(root, root);
    }

    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    template <class F> void path(int u, int v, F f) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
            f(in[top[u]], in[u]);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) std::swap(u, v);
        f(in[u], in[v]);
    }
};

/*
HLD hld; hld.init(n);
for (auto [u, v] : edges) hld.add_edge(u, v);
hld.build(root); int p = hld.lca(u, v);
std::vector<i64> b(n + 1);
for (int u = 1; u <= n; u ++) b[hld.in[u]] = a[u];
SegTree seg; seg.init(b); i64 ans = 0;
hld.path(u, v, [&](int l, int r) { ans += seg.query(l, r); });
*/
