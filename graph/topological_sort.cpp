#include <bits/stdc++.h>
using namespace std;

// 有向图拓扑排序；graph 使用 1-based 顶点编号，存在环时返回 nullopt。
// 复杂度 O(n + m)，空间 O(n)。
optional<vector<int>> topological_sort(const vector<vector<int>> &graph) {
    int n = int(graph.size()) - 1;
    vector<int> indegree(n + 1);
    for (int u = 1; u <= n; ++u)
        for (int v : graph[u]) ++indegree[v];

    queue<int> ready;
    for (int u = 1; u <= n; ++u) if (indegree[u] == 0) ready.push(u);
    vector<int> order;
    order.reserve(n);
    while (!ready.empty()) {
        int u = ready.front();
        ready.pop();
        order.push_back(u);
        for (int v : graph[u]) if (--indegree[v] == 0) ready.push(v);
    }
    if (int(order.size()) != n) return nullopt;
    return order;
}

// Usage:
// optional<vector<int>> order = topological_sort(graph);
