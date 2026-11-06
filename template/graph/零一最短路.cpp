#include <bits/stdc++.h>

#define V vector

using namespace std;

// 边权仅 0 / 1；g[u] 存 {v, w}，顶点 1-based；不可达为 INT_MAX / 2。
// 时间 O(n + m)，空间 O(n + m)，自环重边允许。
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

// 用法：auto dis = bfs01(g, s);
