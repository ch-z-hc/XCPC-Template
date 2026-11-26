// 求树上每个点到所有点的距离和，O(n)。输入为 1-based 连通树，可有负边权，距离及乘积须放入 i64。

#include <bits/stdc++.h>

using i64 = long long;

using Graph = std::vector<std::vector<std::pair<int, i64>>>;

std::vector<i64> reroot(const Graph &g) {
    int n = int(g.size()) - 1;
    if (n <= 0) return {};
    std::vector<i64> siz(n + 1, 1), sum(n + 1), ans(n + 1);
    auto dfs1 = [&](auto &&self, int u, int fa) -> void {
        for (auto [v, w] : g[u])
            if (v != fa) {
                self(self, v, u);
                siz[u] += siz[v];
                sum[u] += sum[v] + siz[v] * w;
            }
    };
    auto dfs2 = [&](auto &&self, int u, int fa) -> void {
        for (auto [v, w] : g[u])
            if (v != fa) {
                ans[v] = ans[u] + (i64(n) - 2 * siz[v]) * w;
                self(self, v, u);
            }
    };
    dfs1(dfs1, 1, 0);
    ans[1] = sum[1];
    dfs2(dfs2, 1, 0);
    return ans;
}

// std::vector<i64> sum = reroot(g);
