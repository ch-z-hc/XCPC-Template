#include <bits/stdc++.h>

#define V vector

using namespace std;

// Euler Tour + RMQ，输入为连通树，顶点编号 1-based，欧拉序位置 0-based。
// 预处理 O(n log n)，查询 O(1)，空间 O(n log n)；递归深度最坏 O(n)。

struct EulerLCA {
    int n = 0;
    V < V < int > > g;
    V < int > pos, dep, ord, lg;
    V < V < int > > st;

    EulerLCA(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        pos.assign(n + 1, -1);
        dep.assign(n + 1, 0);
        ord.clear();
        lg.clear();
        st.clear();
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa, int d) {
        dep[u] = d;
        pos[u] = int(ord.size());
        ord.push_back(u);
        for (int v : g[u]) {
            if (v == fa) continue;
            dfs(v, u, d + 1);
            ord.push_back(u);
        }
    }

    void build(int root = 1) {
        ord.clear();
        fill(pos.begin(), pos.end(), -1);
        dfs(root, 0, 0);
        int m = int(ord.size());
        lg.assign(m + 1, 0);
        for (int i = 2; i <= m; ++ i) lg[i] = lg[i >> 1] + 1;
        st.assign(lg[m] + 1, V < int > (m));
        iota(st[0].begin(), st[0].end(), 0);
        for (int j = 1; j <= lg[m]; ++ j)
            for (int i = 0; i + (1 << j) <= m; ++ i) {
                int x = st[j - 1][i];
                int y = st[j - 1][i + (1 << (j - 1))];
                st[j][i] = dep[ord[x]] < dep[ord[y]] ? x : y;
            }
    }

    int lca(int u, int v) {
        int l = pos[u], r = pos[v];
        if (l > r) swap(l, r);
        int k = lg[r - l + 1];
        int x = st[k][l], y = st[k][r - (1 << k) + 1];
        return dep[ord[x]] < dep[ord[y]] ? ord[x] : ord[y];
    }
};

// 用法：EulerLCA lca(n);
// lca.add_edge(u, v);
// lca.build(root);
// int p = lca.lca(u, v);
