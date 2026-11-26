/*
无向图点双分解，O(n + m)。允许重边，不能有自环，孤立点自成一块。边按加入顺序从 0 编号。
bcc[i].v 存顶点，bcc[i].e 存边号，割点可属于多个分量。
*/

#include <bits/stdc++.h>

struct VBCC {
    struct Block {
        std::vector<int> v, e;
    };

    int n = 0, tim = 0;
    std::vector<std::vector<std::pair<int, int>>> g;
    std::vector<int> dfn, low, stk, mark;
    std::vector<std::pair<int, int>> edges;
    int token = 0;
    std::vector<Block> bcc;

    VBCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tim = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        stk.clear();
        bcc.clear();
        edges.clear();
        mark.assign(n + 1, 0);
        token = 0;
    }

    int add_edge(int u, int v) {
        int id = int(edges.size());
        edges.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
        return id;
    }

    void collect(int stop) {
        std::vector<int> v, e;
        ++ token;
        while (true) {
            int id = stk.back();
            stk.pop_back();
            e.push_back(id);
            for (int side = 0; side < 2; side ++) {
                int u = side == 0 ? edges[id].first : edges[id].second;
                if (mark[u] != token) mark[u] = token, v.push_back(u);
            }
            if (id == stop) break;
        }
        bcc.push_back({});
        bcc.back().v.swap(v);
        bcc.back().e.swap(e);
    }

    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++ tim;
        for (auto [v, id] : g[u]) {
            if (id == pe) continue;
            if (!dfn[v]) {
                stk.push_back(id);
                dfs(v, id);
                low[u] = std::min(low[u], low[v]);
                if (low[v] >= dfn[u]) collect(id);
            } else if (dfn[v] < dfn[u]) {
                stk.push_back(id);
                low[u] = std::min(low[u], dfn[v]);
            }
        }
    }

    void build() {
        tim = 0;
        std::fill(dfn.begin(), dfn.end(), 0);
        std::fill(low.begin(), low.end(), 0);
        std::fill(mark.begin(), mark.end(), 0);
        stk.clear();
        bcc.clear();
        token = 0;
        for (int u = 1; u <= n; u ++) {
            if (!dfn[u]) {
                dfs(u, -1);
                if (g[u].empty()) bcc.push_back({{u}, {}});
            }
        }
    }
};

/*
VBCC vbcc; vbcc.init(n);
vbcc.add_edge(u, v);
vbcc.build();
*/
