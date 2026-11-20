#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

typedef V < V < pair < int, ll > > > Graph;

// 从 s 出发求最短路。点编号 1..n，g[u] 存 {终点, 边权}，所有边权须非负。
// 不可达点的 dis 为 inf。有限距离须小于 inf，松弛时 d + w 不能溢出 ll。
// 二叉堆可保存重复项，时间 O((n + m) log(n + m))，空间 O(n + m)。
V < ll > dijkstra(const Graph &g, int s, ll inf = (1ll << 62)) {
    int n = int(g.size()) - 1;
    V < ll > dis(n + 1, inf);
    priority_queue < pair < ll, int >, V < pair < ll, int > >,
                     greater < pair < ll, int > > > q;
    dis[s] = 0;
    q.push({0, s});
    while (! q.empty()) {
        auto [d, u] = q.top(); q.pop();
        if (d != dis[u]) continue;
        for (auto [v, w] : g[u]) if (dis[v] > d + w) {
            dis[v] = d + w;
            q.push({dis[v], v});
        }
    }
    return dis;
}

// Graph g(n + 1);
// g[u].push_back({v, w}); // 无向边还需添加反向边。
// V < ll > dis = dijkstra(g, s);
