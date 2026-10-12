#include <bits/stdc++.h>

#define V vector

using namespace std;

// 无向图点双连通分量（顶点双连通分量）。
// 顶点编号 1-based，支持重边但不含自环；build 复杂度 O(n + m)，空间 O(n + m)。
// bcc 保存每个点双中的顶点和边（边编号从 0 开始）；孤立点单独形成一个分量。

struct VBCC {
    struct Block {
        V < int > v, e;
    };

    int n = 0, tim = 0;
    V < V < pair < int, int > > > g;
    V < int > dfn, low, stk, mark;
    V < pair < int, int > > edges;
    int token = 0;
    V < Block > bcc;

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
        V < int > v, e;
        ++ token;
        while (true) {
            int id = stk.back();
            stk.pop_back();
            e.push_back(id);
            for (int side = 0; side < 2; ++ side) {
                int u = side == 0 ? edges[id].first : edges[id].second;
                if (mark[u] != token)
                    mark[u] = token, v.push_back(u);
            }
            if (id == stop) break;
        }
        bcc.push_back({});
        bcc.back().v.swap(v); bcc.back().e.swap(e);
    }

    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++ tim;
        for (auto [v, id] : g[u]) {
            if (id == pe) continue;
            if (! dfn[v]) {
                stk.push_back(id);
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) collect(id);
            } else if (dfn[v] < dfn[u]) {
                stk.push_back(id);
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void build() {
        tim = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(mark.begin(), mark.end(), 0);
        stk.clear();
        bcc.clear();
        token = 0;
        for (int u = 1; u <= n; ++ u) {
            if (! dfn[u]) {
                dfs(u, -1);
                if (g[u].empty()) bcc.push_back({{u}, {}});
            }
        }
    }
};

// 调用示例：
// VBCC vbcc(n);
// vbcc.add_edge(u, v);
// vbcc.build(); // vbcc.bcc 保存各分量。
