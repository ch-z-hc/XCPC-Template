// 非负边权单源最短路，O((n + m) log(n + m))。不可达记 inf，有限距离 < inf，松弛和须放入 i64。

#include <bits/stdc++.h>

using i64 = long long;

using Graph = std::vector<std::vector<std::pair<int, i64>>>;

std::vector<i64> dijkstra(const Graph &g, int s, i64 inf = (1ll << 62)) {
    int n = int(g.size()) - 1;
    std::vector<i64> dis(n + 1, inf);
    std::priority_queue<std::pair<i64, int>, std::vector<std::pair<i64, int>>,
                        std::greater<std::pair<i64, int>>>
        q;
    dis[s] = 0;
    q.push({0, s});
    while (!q.empty()) {
        auto [d, u] = q.top();
        q.pop();
        if (d != dis[u]) continue;
        for (auto [v, w] : g[u])
            if (dis[v] > d + w) {
                dis[v] = d + w;
                q.push({dis[v], v});
            }
    }
    return dis;
}

/*
Graph g(n + 1);
g[u].push_back({v, w});
std::vector<i64> dis = dijkstra(g, s);
*/
