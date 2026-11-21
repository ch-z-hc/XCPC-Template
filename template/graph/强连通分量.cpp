#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
有向图顶点编号 1..n。build() 返回分量数，bel[u] 是点 u 所在分量的编号，从 1 数。
若有跨分量边 u -> v，则 bel[u] > bel[v]。按编号从大到小就是缩点图的拓扑序。
可重复 build。时间和空间 O(n + m)，长链的 DFS 最深 n 层。
*/
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

/*
SCC scc(n);
scc.add_edge(u, v);
int cnt = scc.build();
int id = scc.bel[u];
*/
