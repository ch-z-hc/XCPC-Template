#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
顶点和 DFS 序都从 1 数。子树对应 [in[u], out[u]]，path 把树上路径拆成若干闭区间。
每段都含两端，整条路径含 LCA。维护边权时要跳过 LCA 对应的位置。
回调顺序不保证沿 u 到 v 的方向，适合求和、最值。建树 O(n)，每条路径 O(log n) 段。
lca 查询 O(log n)，空间 O(n)。输入须为连通树，长链 DFS 最深 n 层。
*/

struct HLD {
    int n = 0, tim = 0;
    V < V < int > > g;
    V < int > fa, dep, siz, son, top, in, out, rev;

    HLD(int n = 0) { init(n); }
    void init(int n_) {
        n = n_; tim = 0;
        g.assign(n + 1, {});
        fa.assign(n + 1, 0); dep.assign(n + 1, 0);
        siz.assign(n + 1, 0); son.assign(n + 1, 0);
        top.assign(n + 1, 0); in.assign(n + 1, 0);
        out.assign(n + 1, 0); rev.assign(n + 1, 0);
    }

    void add_edge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }

    void dfs1(int u, int p) {
        fa[u] = p;
        siz[u] = 1;
        son[u] = 0;
        for (int v : g[u]) if (v != p) {
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
        tim = 0; dep[root] = 0;
        dfs1(root, 0); dfs2(root, root);
    }

    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    template < class F > void path(int u, int v, F f) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            f(in[top[u]], in[u]);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        f(in[u], in[v]);
    }
};

/*
HLD hld(n);
for (auto [u, v] : edges) hld.add_edge(u, v);
hld.build(root);
// 求路径点权和时，先复制「线段树与树上二分」，按 DFS 序建线段树。
V < ll > b(n + 1);
for (int u = 1; u <= n; ++ u) b[hld.in[u]] = a[u];
SegTree seg; seg.init(b); ll ans = 0;
hld.path(u, v, [&](int l, int r) { ans += seg.query(l, r); });
*/
