#include <bits/stdc++.h>
using namespace std;

// Tarjan 强连通分量；component[u] 为顶点 u 所属编号。
// 顶点编号 1-based；build 复杂度 O(n + m)，空间 O(n + m)。
struct SCC {
    int n = 0, timer = 0, count = 0;
    vector<vector<int>> graph;
    vector<int> dfn, low, component, stack_nodes;
    vector<char> in_stack;
    explicit SCC(int n = 0) { init(n); }
    void init(int n_) { n = n_; timer = count = 0; graph.assign(n + 1, {}); }
    void add_edge(int u, int v) { graph[u].push_back(v); }
    void dfs(int u) {
        dfn[u] = low[u] = ++timer; stack_nodes.push_back(u); in_stack[u] = 1;
        for (int v : graph[u]) if (!dfn[v]) dfs(v), low[u] = min(low[u], low[v]);
            else if (in_stack[v]) low[u] = min(low[u], dfn[v]);
        if (dfn[u] == low[u]) { ++count; while (true) { int v = stack_nodes.back(); stack_nodes.pop_back(); in_stack[v] = 0; component[v] = count; if (v == u) break; } }
    }
    int build() {
        timer = count = 0;
        dfn.assign(n + 1, 0); low.assign(n + 1, 0); component.assign(n + 1, 0); in_stack.assign(n + 1, 0); stack_nodes.clear();
        for (int u = 1; u <= n; ++u) if (!dfn[u]) dfs(u);
        return count;
    }
};
