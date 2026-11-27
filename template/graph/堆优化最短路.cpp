// 非负边权单源最短路，O((n + m) log(n + m))。不可达记 inf，有限距离 < inf，松弛和须放入 ll。

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

using Graph = V < V < pair < int, ll > > >;

V < ll > dijkstra(const Graph &g, int s, ll inf = (1ll << 62)) {
    int n = int(g.size()) - 1;
    V < ll > dis(n + 1, inf);
    priority_queue < pair < ll, int >, V < pair < ll, int > >,
                        greater < pair < ll, int > > >
        q;
    dis[s] = 0;
    q.push({0, s});
    while (!q.empty()) {
        auto [d, u] = q.top(); q.pop();
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
V < ll > dis = dijkstra(g, s);
*/
