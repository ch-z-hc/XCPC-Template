#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

typedef V < V < pair < int, ll > > > Graph;

// 非负边权单源最短路；图和距离均为 1-based，g[u] 保存 {v, w}。
// 使用二叉堆，复杂度 O((n + m) log n)，空间 O(n + m)。
// 要求路径计算不溢出 ll，有效距离小于 inf；不可达返回 inf，不使用模数。
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

// 调用示例：
// Graph g(n + 1);
// g[u].push_back({v, w}); // 无向边还需添加反向边。
// V < ll > dis = dijkstra(g, s);
