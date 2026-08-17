#include <bits/stdc++.h>
using namespace std;

// 无向图欧拉路。edges 按输入顺序编号，返回顶点序列；不存在时返回空。
// 顶点编号 1-based；复杂度 O(n + m)，空间 O(n + m)。
vector<int> undirected_euler_trail(int n, const vector<pair<int, int>> &edges) {
    vector<vector<pair<int, int>>> graph(n + 1);
    vector<int> degree(n + 1), ptr(n + 1);
    for (int i = 0; i < int(edges.size()); ++i) {
        auto [u, v] = edges[i];
        graph[u].push_back({v, i}); graph[v].push_back({u, i});
        ++degree[u]; ++degree[v];
    }
    int start = 1, odd = 0;
    for (int u = 1; u <= n; ++u) if (degree[u] & 1) {
        ++odd; start = u;
    }
    if (odd != 0 && odd != 2) return {};
    if (edges.empty()) return {1};
    if (odd == 0) {
        while (start <= n && graph[start].empty()) ++start;
        if (start > n) return {};
    }
    vector<char> used(edges.size());
    vector<int> answer, st{start};
    while (!st.empty()) {
        int u = st.back();
        while (ptr[u] < int(graph[u].size()) && used[graph[u][ptr[u]].second]) ++ptr[u];
        if (ptr[u] == int(graph[u].size())) answer.push_back(u), st.pop_back();
        else {
            auto [v, id] = graph[u][ptr[u]++];
            if (!used[id]) used[id] = 1, st.push_back(v);
        }
    }
    if (int(answer.size()) != int(edges.size()) + 1) return {};
    reverse(answer.begin(), answer.end());
    return answer;
}
