// 非负权连通树的最远点与直径，分别为 O(n)。距离须放入 i64，长链注意递归栈。

#include <bits/stdc++.h>

using i64 = long long;

using Graph = std::vector<std::vector<std::pair<int, i64>>>;

std::pair<int, i64> tree_farthest(int s, const Graph &g) {
    std::pair<int, i64> ans = {s, 0};
    auto dfs = [&](auto &&self, int u, int fa, i64 dis) -> void {
        if (dis > ans.second) ans = {u, dis};
        for (int i = int(g[u].size()) - 1; i >= 0; i --) {
            auto [v, w] = g[u][i];
            if (v != fa) self(self, v, u, dis + w);
        }
    };
    dfs(dfs, s, 0, 0);
    return ans;
}

std::tuple<int, int, i64> tree_diameter(const Graph &g) {
    if (g.size() <= 1) return {0, 0, 0};
    int u = tree_farthest(1, g).first;
    auto [v, len] = tree_farthest(u, g);
    return {u, v, len};
}

/*
Graph g(3); g[1].push_back({2, 7}); g[2].push_back({1, 7});
auto [far, dist] = tree_farthest(1, g);
auto [u, v, len] = tree_diameter(g);
*/
