#include <bits/stdc++.h>
using namespace std;

// 无向图割点；顶点编号从 1 开始。
struct ArticulationFinder {
    int n = 0, timer = 0;
    vector<vector<int>> graph;
    vector<int> dfn, low;
    vector<char> is_cut;

    explicit ArticulationFinder(int n = 0) { init(n); }
    void init(int n_) {
        n = n_; timer = 0;
        graph.assign(n + 1, {});
        dfn.assign(n + 1, 0); low.assign(n + 1, 0);
        is_cut.assign(n + 1, 0);
    }
    void add_edge(int u, int v) {
        graph[u].push_back(v); graph[v].push_back(u);
    }
    void dfs(int u, int parent) {
        dfn[u] = low[u] = ++timer;
        int children = 0;
        for (int v : graph[u]) {
            if (!dfn[v]) {
                ++children; dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (parent != 0 && low[v] >= dfn[u]) is_cut[u] = 1;
            } else if (v != parent) low[u] = min(low[u], dfn[v]);
        }
        if (parent == 0 && children > 1) is_cut[u] = 1;
    }
    vector<int> find() {
        timer = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_cut.begin(), is_cut.end(), 0);
        for (int u = 1; u <= n; ++u) if (!dfn[u]) dfs(u, 0);
        vector<int> answer;
        for (int u = 1; u <= n; ++u) if (is_cut[u]) answer.push_back(u);
        return answer;
    }
};
