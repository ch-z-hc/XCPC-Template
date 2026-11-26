/*
无向图欧拉路，O(n + m)。顶点从 1 编号，n >= 1，每条边只给一次。最深递归 O(m)。
返回顶点序列，无解时为空，无边时为 {1}。
*/

#include <bits/stdc++.h>

std::vector<int> euler_u(int n, const std::vector<std::pair<int, int>> &edges) {
    std::vector<std::vector<std::pair<int, int>>> g(n + 1);
    std::vector<int> deg(n + 1), cur(n + 1);
    for (int i = 0; i < int(edges.size()); i ++) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        g[v].push_back({u, i});
        ++ deg[u];
        ++ deg[v];
    }
    int s = 1, cnt = 0;
    for (int u = 1; u <= n; u ++)
        if (deg[u] & 1) {
            ++ cnt;
            s = u;
        }
    if (cnt != 0 && cnt != 2) return {};
    if (edges.empty()) return {1};
    if (cnt == 0) {
        while (s <= n && g[s].empty()) ++ s;
        if (s > n) return {};
    }
    std::vector<char> vis(edges.size());
    std::vector<int> ans;
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
    std::reverse(ans.begin(), ans.end());
    return ans;
}

// auto path = euler_u(3, {{1, 2}, {2, 3}});
