#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 换根 DP 示例：求树上每个点到所有点的距离和。
// 输入必须是连通树，顶点编号 1-based，边权可为任意整数；复杂度 O(n)。
// 两次递归 DFS，递归深度最坏 O(n)。

typedef V < V < pair < int, ll > > > Graph;

V < ll > reroot(const Graph &g) {
    int n = int(g.size()) - 1;
    if (n <= 0) return {};
    V < ll > siz(n + 1, 1), sum(n + 1), ans(n + 1);
    auto dfs1 = [&](auto &&self, int u, int fa) -> void {
        for (auto [v, w] : g[u]) if (v != fa) {
            self(self, v, u);
            siz[u] += siz[v];
            sum[u] += sum[v] + siz[v] * w;
        }
    };
    auto dfs2 = [&](auto &&self, int u, int fa) -> void {
        for (auto [v, w] : g[u]) if (v != fa) {
            ans[v] = ans[u] + (static_cast<ll> (n) - 2 * siz[v]) * w;
            self(self, v, u);
        }
    };
    dfs1(dfs1, 1, 0);
    ans[1] = sum[1];
    dfs2(dfs2, 1, 0);
    return ans;
}

// 调用示例：
// V < ll > sum = reroot(g);
