// 求无向图全部桥，O(n + m)。边须有唯一编号 1..m，重边分别编号，返回的边号递增。

#include <bits/stdc++.h>

#define V vector
#define pii pair < int, int >

using namespace std;

struct Bridge {
    int n = 0, tim = 0;
    V < V < pii > > g;
    V < int > dfn, low;
    V < char > is_bridge;

    Bridge(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        tim = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0); low.assign(n + 1, 0);
        is_bridge.clear();
    }
    void add_edge(int id, int u, int v) {
        if (int(is_bridge.size()) <= id) is_bridge.resize(id + 1);
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }
    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++ tim;
        for (auto [v, id] : g[u]) {
            if (id == pe) continue;
            if (!dfn[v]) {
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) is_bridge[id] = 1;
            } else
                low[u] = min(low[u], dfn[v]);
        }
    }
    V < int > find() {
        tim = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_bridge.begin(), is_bridge.end(), 0);
        for (int u = 1; u <= n; u ++)
            if (!dfn[u]) dfs(u, -1);
        V < int > ans;
        for (int id = 1; id < int(is_bridge.size()); id ++)
            if (is_bridge[id]) ans.push_back(id);
        return ans;
    }
};

/*
Bridge bridge; bridge.init(n);
bridge.add_edge(id, u, v);
V < int > ans = bridge.find();
*/
