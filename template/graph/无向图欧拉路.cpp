#include <bits/stdc++.h>

#define V vector

using namespace std;

// 无向图顶点编号 1..n，n >= 1。每条边在 edges 中放一次。
// 返回经过每条边恰好一次的顶点序列，无解返回空数组，无边返回 {1}。
// 时间和空间 O(n + m)，递归最深 O(m)。

V < int > euler_u(int n, const V < pair < int, int > > &edges) {
    V < V < pair < int, int > > > g(n + 1);
    V < int > deg(n + 1), cur(n + 1);
    for (int i = 0; i < int(edges.size()); ++ i) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i}); g[v].push_back({u, i});
        ++ deg[u]; ++ deg[v];
    }
    int s = 1, cnt = 0;
    for (int u = 1; u <= n; ++ u) if (deg[u] & 1) {
        ++ cnt; s = u;
    }
    if (cnt != 0 && cnt != 2) return {};
    if (edges.empty()) return {1};
    if (cnt == 0) {
        while (s <= n && g[s].empty()) ++ s;
        if (s > n) return {};
    }
    V < char > vis(edges.size());
    V < int > ans;
    auto dfs = [&](auto &&self, int u) -> void {
        while (cur[u] < int(g[u].size())) {
            auto [v, id] = g[u][cur[u] ++];
            if (vis[id]) continue;
            vis[id] = 1;
            self(self, v);
        }
        ans.push_back(u);
    };
    dfs(dfs, s);
    if (int(ans.size()) != int(edges.size()) + 1) return {};
    reverse(ans.begin(), ans.end());
    return ans;
}

// V < int > path = euler_u(n, edges);
