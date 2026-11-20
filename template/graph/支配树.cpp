#include <bits/stdc++.h>

#define V vector

using namespace std;

// 给有向图加完边后调用 build(root)。idom[v] 是 v 的直接支配点，根指向自己，不可达点为 0。
// 点编号 1..n，tree[u] 存支配树的儿子。可更换根重建。长链 DFS 最深 n 层。
// 时间 O((n + m) log n)，空间 O(n + m)。
struct Dominator {
    int n = 0, tim = 0;
    V < V < int > > g, pre, bucket, tree;
    V < int > dfn, rev, par, semi, fa, label, dom, idom;

    Dominator(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        idom.assign(n + 1, 0);
        tree.assign(n + 1, {});
    }

    void add_edge(int u, int v) { g[u].push_back(v); }

    void dfs(int u) {
        dfn[u] = ++ tim; rev[tim] = u;
        for (int v : g[u]) {
            if (! dfn[v]) {
                dfs(v);
                par[dfn[v]] = dfn[u];
            }
            pre[dfn[v]].push_back(dfn[u]);
        }
    }

    void compress(int u) {
        if (! fa[fa[u]]) return;
        compress(fa[u]);
        if (semi[label[fa[u]]] < semi[label[u]]) label[u] = label[fa[u]];
        fa[u] = fa[fa[u]];
    }

    int eval(int u) { compress(u); return label[u]; }

    void build(int root) {
        tim = 0;
        dfn.assign(n + 1, 0); rev.assign(n + 1, 0);
        par.assign(n + 1, 0); fa.assign(n + 1, 0);
        dom.assign(n + 1, 0); idom.assign(n + 1, 0);
        semi.resize(n + 1); label.resize(n + 1);
        iota(semi.begin(), semi.end(), 0);
        iota(label.begin(), label.end(), 0);
        pre.assign(n + 1, {}); bucket.assign(n + 1, {});
        tree.assign(n + 1, {});
        dfs(root);
        for (int u = tim; u >= 2; -- u) {
            for (int v : pre[u]) semi[u] = min(semi[u], semi[eval(v)]);
            bucket[semi[u]].push_back(u);
            fa[u] = par[u];
            for (int v : bucket[par[u]]) {
                int w = eval(v);
                dom[v] = semi[w] < semi[v] ? w : par[u];
            }
            bucket[par[u]].clear();
        }
        idom[root] = root;
        for (int u = 2; u <= tim; ++ u) {
            if (dom[u] != semi[u]) dom[u] = dom[dom[u]];
            idom[rev[u]] = rev[dom[u]];
            tree[idom[rev[u]]].push_back(rev[u]);
        }
    }
};

// Dominator dt(n); dt.add_edge(u, v); dt.build(root);
// int parent = dt.idom[v];
