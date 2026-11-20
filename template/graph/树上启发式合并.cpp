#include <bits/stdc++.h>

#define V vector

using namespace std;

// 树上顶点编号 1..n。按题意补全 add(u)、del(u)、query(u)，前两个操作须互为逆操作。
// 执行 query(u) 时，当前恰好保留 u 的整棵子树。build 前清空统计量，结束时所有点已移除。
// 最多 O(n log n) 次增删、n 次查询，空间 O(n)，长链递归深度 O(n)。

struct DSUOnTree {
    int n = 0;
    V < V < int > > g;
    V < int > siz, son, in, out, ord;

    // 示例维护：用以下成员替换下方 3 个函数声明。
    // V < int > color, cnt, ans; int kinds = 0;
    // void add(int u) { if (cnt[color[u]] ++ == 0) ++ kinds; }
    // void del(int u) { if (-- cnt[color[u]] == 0) -- kinds; }
    // void query(int u) { ans[u] = kinds; }
    void add(int u);
    void del(int u);
    void query(int u);

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

    void build(int root = 1) {
        ord.clear();
        fill(siz.begin(), siz.end(), 0);
        fill(son.begin(), son.end(), 0);
        dfs_size(root, 0);
        dfs(root, 0, false);
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

    void dfs(int u, int fa, bool keep) {
        for (int v : g[u])
            if (v != fa && v != son[u])
                dfs(v, u, false);

        if (son[u])
            dfs(son[u], u, true);

        for (int v : g[u]) if (v != fa && v != son[u])
            for (int i = in[v]; i < out[v]; ++ i) add(ord[i]);
        add(u);
        query(u);

        if (! keep)
            for (int i = in[u]; i < out[u]; ++ i) del(ord[i]);
    }
};

// DSUOnTree dsu(n);
// for (auto [u, v] : edges) dsu.add_edge(u, v);
// color[u] 已离散到 1..m。
// dsu.color = color; dsu.cnt.assign(m + 1, 0); dsu.kinds = 0;
// dsu.ans.resize(n + 1); dsu.build(1); // 顶点 u 的答案为 dsu.ans[u]。
