#include <bits/stdc++.h>

#define V vector

using namespace std;

// 树上莫队：静态树、路径上的点统计；顶点 1-based，边必须组成一棵树。
// build(root) 递归构造双倍欧拉序及 LCA，之后 query(u, v) 返回答案编号。
// solve(add_vertex, del_vertex, answer)；回调状态初始为空，新增边后重建。
// 预处理 O(n log n)，q 次查询 O(q log q + n sqrt(q) + q)，空间 O(n log n + q)。
struct MoTree {
    struct Query { int l, r, extra, id; };
    int n, lg, timer;
    V < V < int > > g, up;
    V < int > in, out, ord, dep;
    V < Query > qs;

    MoTree(int n = 0) { init(n); }
    void init(int n_) {
        n = n_; g.assign(n + 1, {}); qs.clear();
    }
    void add_edge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }
    void dfs(int u, int fa) {
        up[0][u] = fa; dep[u] = dep[fa] + 1;
        for (int j = 1; j < lg; ++ j) up[j][u] = up[j - 1][up[j - 1][u]];
        in[u] = ++ timer; ord[timer] = u;
        for (int v : g[u]) if (v != fa) dfs(v, u);
        out[u] = ++ timer; ord[timer] = u;
    }
    void build(int root = 1) {
        lg = 1; while ((1ll << lg) <= n) lg ++;
        up.assign(lg, V < int > (n + 1));
        in.assign(n + 1, 0); out = dep = in;
        ord.assign(2 * n + 1, 0); timer = 0; qs.clear();
        if (n) dfs(root, 0);
    }
    int lca(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int j = 0; j < lg; ++ j) if (d >> j & 1) u = up[j][u];
        if (u == v) return u;
        for (int j = lg - 1; j >= 0; -- j)
            if (up[j][u] != up[j][v]) u = up[j][u], v = up[j][v];
        return up[0][u];
    }
    int query(int u, int v) {
        if (in[u] > in[v]) swap(u, v);
        int z = lca(u, v), id = int(qs.size());
        if (z == u) qs.push_back({in[u], in[v], 0, id});
        else qs.push_back({out[u], in[v], z, id});
        return id;
    }
    template < class Add, class Remove, class Answer >
    void solve(Add add, Remove del, Answer answer) const {
        auto queries = qs;
        int B = max(1, int(2 * n / max(1.0, sqrt(double(qs.size())))));
        sort(queries.begin(), queries.end(), [&](Query a, Query b) {
            if (a.l / B != b.l / B) return a.l / B < b.l / B;
            return (a.l / B & 1) ? a.r > b.r : a.r < b.r;
        });
        V < int > vis(n + 1);
        auto toggle = [&](int u) { if (vis[u]) del(u); else add(u); vis[u] ^= 1; };
        int l = 1, r = 0;
        for (auto q : queries) {
            while (l > q.l) toggle(ord[-- l]);
            while (r < q.r) toggle(ord[++ r]);
            while (l < q.l) toggle(ord[l ++]);
            while (r > q.r) toggle(ord[r --]);
            if (q.extra) toggle(q.extra);
            answer(q.id);
            if (q.extra) toggle(q.extra);
        }
    }
};

// MoTree mo(n); mo.add_edge(u, v); mo.build(); int id = mo.query(u, v);
// mo.solve(add_vertex, del_vertex, answer);
