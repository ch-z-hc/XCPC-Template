#include <bits/stdc++.h>

#define V vector

using namespace std;

// Tarjan 强连通分量；顶点编号 1-based，bel[u] 为所属分量编号（从 1 开始）。
// build 为 O(n + m)，空间 O(n + m)；递归深度最坏为 O(n)。
// build 可重复调用，返回分量数；分量编号按出栈顺序，不是拓扑序。
struct SCC {
    int n = 0, tim = 0, cnt = 0;
    V < V < int > > g;
    V < int > dfn, low, bel, stk;
    V < char > vis;

    SCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tim = cnt = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        bel.assign(n + 1, 0);
        vis.assign(n + 1, 0);
        stk.clear();
    }

    void add_edge(int u, int v) { g[u].push_back(v); }

    void dfs(int u) {
        dfn[u] = low[u] = ++ tim;
        stk.push_back(u);
        vis[u] = 1;
        for (int v : g[u]) {
            if (! dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (vis[v]) low[u] = min(low[u], dfn[v]);
        }
        if (dfn[u] != low[u]) return;
        cnt ++;
        while (1) {
            int v = stk.back(); stk.pop_back();
            vis[v] = 0;
            bel[v] = cnt;
            if (u == v) break;
        }
    }

    int build() {
        tim = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        fill(vis.begin(), vis.end(), 0);
        stk.clear();
        for (int u = 1; u <= n; ++ u)
            if (! dfn[u]) dfs(u);
        return cnt;
    }
};

// 调用示例：
// SCC scc(n);
// scc.add_edge(u, v);
// int cnt = scc.build();
// int id = scc.bel[u];
