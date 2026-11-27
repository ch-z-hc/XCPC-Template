// 边权只能是 0 或 1 的单源最短路，O(n + m)。邻接表按 {终点, 边权} 存储，不可达记 INT_MAX / 2。

#include <bits/stdc++.h>

#define V vector
#define pii pair < int, int >
using namespace std;

V < int > bfs01(const V < V < pii > > &g, int s) {
    V < int > dis(g.size(), INT_MAX / 2), vis(g.size());
    deque < int > q;
    dis[s] = 0;
    q.push_back(s);
    while (!q.empty()) {
        int u = q.front(); q.pop_front();
        if (vis[u]) continue;
        vis[u] = 1;
        for (auto [v, w] : g[u])
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (w) q.push_back(v);
                else q.push_front(v);
            }
    }
    return dis;
}

// auto dis = bfs01(g, s);
