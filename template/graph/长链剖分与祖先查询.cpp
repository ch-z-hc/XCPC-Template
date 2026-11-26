// 求树上第 k 级祖先，建树 O(n log n)，查询 O(1)。k >= 0，越过根返回 0，根深度为 1。

#include <bits/stdc++.h>

struct KthAncestor {
    int n = 0, lg = 1;
    std::vector<std::vector<int>> g, up, above, below;
    std::vector<int> dep, len, son, top;

    KthAncestor(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        lg = 1;
        while ((1ll << lg) <= std::max(1, n)) lg ++;
        g.assign(n + 1, {});
    }
    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        up[0][u] = p;
        dep[u] = dep[p] + 1;
        len[u] = 1;
        for (int j = 1; j < lg; j ++) up[j][u] = up[j - 1][up[j - 1][u]];
        for (int v : g[u])
            if (v != p) {
                dfs(v, u);
                if (len[v] + 1 > len[u]) {
                    len[u] = len[v] + 1;
                    son[u] = v;
                }
            }
    }

    void decompose(int u, int head) {
        top[u] = head;
        if (son[u]) decompose(son[u], head);
        for (int v : g[u])
            if (v != up[0][u] && v != son[u]) decompose(v, v);
    }

    void build(int root = 1) {
        up.assign(lg, std::vector<int>(n + 1));
        dep.assign(n + 1, 0);
        len.assign(n + 1, 0);
        son.assign(n + 1, 0);
        top.assign(n + 1, 0);
        above.assign(n + 1, {});
        below.assign(n + 1, {});
        if (!n) return;
        dfs(root, 0);
        decompose(root, root);
        for (int head = 1; head <= n; head ++)
            if (top[head] == head) {
                int u = head;
                for (int i = 0; i < len[head] && u; i ++, u = up[0][u])
                    above[head].push_back(u);
                for (u = head; u; u = son[u]) below[head].push_back(u);
            }
    }

    int query(int u, int k) {
        if (!k) return u;
        if (k >= dep[u]) return 0;
        int j = 31 - __builtin_clz(unsigned(k));
        u = up[j][u];
        k -= 1 << j;
        int head = top[u], d = k - (dep[u] - dep[head]);
        return d >= 0 ? above[head][d] : below[head][-d];
    }
};

/*
KthAncestor tree; tree.init(n); tree.add_edge(u, v); tree.build(root);
int p = tree.query(u, k);
*/
