#include <bits/stdc++.h>
using namespace std;

// 无向图桥（割边）；边编号从 1 开始。
// 顶点编号 1-based；find 复杂度 O(n + m)，空间 O(n + m)。
struct BridgeFinder {
    int n = 0, timer = 0;
    vector<vector<pair<int, int>>> graph;
    vector<int> dfn, low;
    vector<char> is_bridge;

    explicit BridgeFinder(int n = 0) { init(n); }
    void init(int n_) {
        n = n_; timer = 0;
        graph.assign(n + 1, {});
        dfn.assign(n + 1, 0); low.assign(n + 1, 0);
        is_bridge.clear();
    }
    void add_edge(int id, int u, int v) {
        if (int(is_bridge.size()) <= id) is_bridge.resize(id + 1);
        graph[u].push_back({v, id});
        graph[v].push_back({u, id});
    }
    void dfs(int u, int parent_edge) {
        dfn[u] = low[u] = ++timer;
        for (auto [v, id] : graph[u]) {
            if (id == parent_edge) continue;
            if (!dfn[v]) {
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) is_bridge[id] = 1;
            } else low[u] = min(low[u], dfn[v]);
        }
    }
    vector<int> find() {
        timer = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_bridge.begin(), is_bridge.end(), 0);
        for (int u = 1; u <= n; ++u) if (!dfn[u]) dfs(u, -1);
        vector<int> answer;
        for (int id = 1; id < int(is_bridge.size()); ++id)
            if (is_bridge[id]) answer.push_back(id);
        return answer;
    }
};
