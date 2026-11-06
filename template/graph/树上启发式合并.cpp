#include <bits/stdc++.h>

#define V vector

using namespace std;

// DSU on Tree：离线处理每个子树的统计问题。
// 使用前补全 add / del / query 的定义及题目状态；顶点编号 1-based。
// add 与 del 必须互为逆操作，query(u) 调用时状态恰为 u 的子树。
// build 前清空统计状态，结束后所有点均已删除，可重复 build。
// 复杂度 O(n log n * T_update + n * T_answer)，空间 O(n)；递归深度最坏为 O(n)。

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

// 用法：DSUOnTree dsu(n);
// dsu.add_edge(u, v);
// 统计子树内不同颜色数，颜色离散到 [1, m]。
// dsu.color = color; dsu.cnt.assign(m + 1, 0); dsu.kinds = 0;
// dsu.ans.resize(n + 1); dsu.build(1); // 顶点 u 的答案为 dsu.ans[u]。
