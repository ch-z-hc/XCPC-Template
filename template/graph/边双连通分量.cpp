/*
无向图边双分解，O(n + m)，允许重边。分量从 1 编号，桥标记按原边从 0 编号。
分量 id 的点及非桥边存于 bcc[id - 1]，is_bridge[e] 标记原边 e。
*/

#include <bits/stdc++.h>

#define V vector
#define pii pair < int, int >

using namespace std;

struct EBCC {
    struct Block {
        V < int > v, e;
    };

    int n = 0, tim = 0;
    V < V < pii > > g;
    V < pii > edges;
    V < int > dfn, low, bel;
    V < char > is_bridge;
    V < char > vis;
    V < Block > bcc;
    int cnt = 0;

    EBCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tim = cnt = 0;
        g.assign(n + 1, {});
        edges.clear();
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        bel.assign(n + 1, 0);
        is_bridge.clear();
        vis.clear();
        bcc.clear();
    }

    int add_edge(int u, int v) {
        int id = int(edges.size());
        edges.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
        return id;
    }

    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++ tim;
        for (auto [v, id] : g[u]) {
            if (id == pe) continue;
            if (!dfn[v]) {
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) is_bridge[id] = 1;
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void paint(int u) {
        bel[u] = cnt;
        bcc.back().v.push_back(u);
        for (auto [v, id] : g[u])
            if (!is_bridge[id]) {
                if (!vis[id]) {
                    vis[id] = 1;
                    bcc.back().e.push_back(id);
                }
                if (!bel[v]) paint(v);
            }
    }

    void build() {
        tim = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        is_bridge.assign(edges.size(), 0);
        vis.assign(edges.size(), 0);
        bcc.clear();
        for (int u = 1; u <= n; u ++)
            if (!dfn[u]) dfs(u, -1);
        for (int u = 1; u <= n; u ++)
            if (!bel[u]) {
                ++ cnt;
                bcc.push_back({});
                paint(u);
            }
    }
};

/*
EBCC ebcc; ebcc.init(n);
ebcc.add_edge(u, v);
ebcc.build();
int id = ebcc.bel[u];
*/
