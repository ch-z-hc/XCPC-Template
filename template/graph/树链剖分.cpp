#include <bits/stdc++.h>

#define V vector

using namespace std;

// 重链剖分，输入为连通树；顶点、DFS 序均为 1-based，区间闭区间。
// build 为 O(n)，lca 为 O(log n)，path 分成 O(log n) 段，空间 O(n)。
// path 回调的段不保证路径方向，适合区间和、最值等与方向无关的操作。
// 子树区间为 [in[u], out[u]]；递归深度最坏 O(n)。区间维护另接线段树。
// path 包含两端和 LCA，下面示例维护点权；边权须额外排除 LCA 对应位置。

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
        fa[u] = p; siz[u] = 1; son[u] = 0;
        for (int v : g[u]) if (v != p) {
            dep[v] = dep[u] + 1;
            dfs1(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[son[u]]) son[u] = v;
        }
    }

    void dfs2(int u, int t) {
        top[u] = t; in[u] = ++ tim; rev[tim] = u;
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

// 调用示例：HLD hld(n); hld.add_edge(u, v); hld.build(root);
// a 为 1-based 点权；先复制数据结构章的“线段树与树上二分”。
// V < ll > b(n + 1);
// for (int u = 1; u <= n; ++ u) b[hld.in[u]] = a[u];
// SegTree seg; seg.init(b); ll ans = 0;
// hld.path(u, v, [&](int l, int r) { ans += seg.query(l, r); });
