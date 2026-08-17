#include <bits/stdc++.h>
using namespace std;

using WeightedGraph = vector<vector<pair<int, long long>>>;

// 非负边权单源最短路；图和距离均为 1-based。
vector<long long> dijkstra(const WeightedGraph &graph, int source,
                           long long inf = (1LL << 62)) {
    int n = int(graph.size()) - 1;
    vector<long long> dist(n + 1, inf);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> que;
    dist[source] = 0;
    que.push({0, source});
    while (!que.empty()) {
        auto [du, u] = que.top(); que.pop();
        if (du != dist[u]) continue;
        for (auto [v, w] : graph[u]) if (dist[v] > du + w) {
            dist[v] = du + w;
            que.push({dist[v], v});
        }
    }
    return dist;
}
