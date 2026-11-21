#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
g[u] 存 {终点, 边权}，点编号 1..n，边权只能是 0 或 1。
bfs01(g, s) 返回从 s 到各点的距离，不可达为 INT_MAX / 2。时间和空间 O(n + m)。
*/
V < int > bfs01(const V < V < pair < int, int > > > &g, int s) {
    V < int > dis(g.size(), INT_MAX / 2), vis(g.size());
    deque < int > q;
    dis[s] = 0; q.push_back(s);
    while (! q.empty()) {
        int u = q.front(); q.pop_front();
        if (vis[u]) continue;
        vis[u] = 1;
        for (auto [v, w] : g[u]) if (dis[v] > dis[u] + w) {
            dis[v] = dis[u] + w;
            if (w) q.push_back(v);
            else q.push_front(v);
        }
    }
    return dis;
}

// auto dis = bfs01(g, s);
