#include <bits/stdc++.h>

#define V vector

using namespace std;

// DSU on Tree：离线处理每个子树的统计问题。
// 适用于加点、删点和读取答案均可由回调维护的状态；顶点编号 1-based。
// add 与 del 必须互为逆操作，query(u) 调用时状态恰为 u 的子树。
// 复杂度 O(n log n * T_update + n * T_answer)，空间 O(n)；递归深度最坏为 O(n)。

struct DSUOnTree {
    int n = 0;
    V < V < int > > g;
    V < int > siz, son, in, out, ord;

    DSUOnTree(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        in.assign(n + 1, 0);
        out.assign(n + 1, 0);
        ord.clear();
        ord.reserve(n);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    template < class Add, class Remove, class Query >
    void build(int root, Add add, Remove del, Query query) {
        ord.clear();
        fill(siz.begin(), siz.end(), 0);
        fill(son.begin(), son.end(), 0);
        dfs_size(root, 0);
        dfs(root, 0, false, add, del, query);
    }

    void dfs_size(int u, int fa) {
        in[u] = int(ord.size());
        ord.push_back(u);
        siz[u] = 1;
        for (int v : g[u]) if (v != fa) {
            dfs_size(v, u);
            siz[u] += siz[v];
            if (! son[u] || siz[v] > siz[son[u]])
                son[u] = v;
        }
        out[u] = int(ord.size());
    }

    template < class Add, class Remove, class Query >
    void dfs(int u, int fa, bool keep, Add &add, Remove &del, Query &query) {
        for (int v : g[u])
            if (v != fa && v != son[u])
                dfs(v, u, false, add, del, query);

        if (son[u])
            dfs(son[u], u, true, add, del, query);

        for (int v : g[u]) if (v != fa && v != son[u])
            for (int i = in[v]; i < out[v]; ++ i) add(ord[i]);
        add(u);
        query(u);

        if (! keep)
            for (int i = in[u]; i < out[u]; ++ i) del(ord[i]);
    }
};

// 调用示例：
// DSUOnTree dsu(n);
// dsu.add_edge(u, v);
// dsu.build(1, add, del, query);
