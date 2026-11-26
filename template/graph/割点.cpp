// 求无向图全部割点，O(n + m)。允许不连通，每条边只加一次，结果按点号递增。

#include <bits/stdc++.h>

#define V vector

using namespace std;

struct CutVertex {
    int n = 0, tim = 0;
    V < V < int > > g;
    V < int > dfn, low;
    V < char > is_cut;

    CutVertex(int n = 0) { init(n); }
    void init(int n_) {
        n = n_; tim = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0); low.assign(n + 1, 0);
        is_cut.assign(n + 1, 0);
    }
    void add_edge(int u, int v) {
        g[u].push_back(v); g[v].push_back(u);
    }
    void dfs(int u, int fa) {
        dfn[u] = low[u] = ++ tim;
        int cnt = 0;
        for (int v : g[u]) {
            if (! dfn[v]) {
                ++ cnt; dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (fa != 0 && low[v] >= dfn[u]) is_cut[u] = 1;
            } else if (v != fa) low[u] = min(low[u], dfn[v]);
        }
        if (fa == 0 && cnt > 1) is_cut[u] = 1;
    }
    V < int > find() {
        tim = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_cut.begin(), is_cut.end(), 0);
        for (int u = 1; u <= n; ++ u) if (! dfn[u]) dfs(u, 0);
        V < int > ans;
        for (int u = 1; u <= n; ++ u) if (is_cut[u]) ans.push_back(u);
        return ans;
    }
};

/*
CutVertex cut; cut.init(n);
cut.add_edge(u, v);
V < int > ans = cut.find();
*/
