#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
返回 sum[u]，即点 u 到树上所有点的距离之和。输入是编号 1..n 的连通树，边权可为负。
距离、乘积和累加结果用 ll。两遍 DFS 共 O(n)，长链递归深度 O(n)。
*/

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
            ans[v] = ans[u] + (ll(n) - 2 * siz[v]) * w;
            self(self, v, u);
        }
    };
    dfs1(dfs1, 1, 0);
    ans[1] = sum[1];
    dfs2(dfs2, 1, 0);
    return ans;
}

// V < ll > sum = reroot(g);
