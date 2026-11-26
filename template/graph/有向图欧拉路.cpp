/*
有向图欧拉路，O(n + m)。顶点从 1 编号，n >= 1，最深递归 O(m)。
返回顶点序列，无解时为空，无边时为 {1}。
*/

#include <bits/stdc++.h>

std::vector<int> euler_d(int n, const std::vector<std::pair<int, int>> &edges) {
    std::vector<std::vector<int>> g(n + 1);
    std::vector<int> in(n + 1), out(n + 1), cur(n + 1);
    for (int i = 0; i < int(edges.size()); i ++) {
        auto [u, v] = edges[i];
        g[u].push_back(v);
        ++ out[u];
        ++ in[v];
    }
    int s = 1, cnt1 = 0, cnt2 = 0;
    for (int u = 1; u <= n; u ++) {
        if (out[u] - in[u] == 1) {
            s = u;
            cnt1 ++;
        } else if (in[u] - out[u] == 1)
            ++ cnt2;
        else if (in[u] != out[u])
            return {};
    }
    if (!((cnt1 == 1 && cnt2 == 1) || (cnt1 == 0 && cnt2 == 0))) return {};
    if (cnt1 == 0) {
        while (s <= n && g[s].empty()) ++ s;
        if (s > n) return edges.empty() ? std::vector<int>{1} : std::vector<int>{};
    }
    std::vector<int> ans;
    auto dfs = [&](auto &&self, int u) -> void {
        while (cur[u] < int(g[u].size())) self(self, g[u][cur[u] ++]);
        ans.push_back(u);
    };
    dfs(dfs, s);
    if (int(ans.size()) != int(edges.size()) + 1) return {};
    std::reverse(ans.begin(), ans.end());
    return ans;
}

// auto path = euler_d(3, {{1, 2}, {2, 3}});
