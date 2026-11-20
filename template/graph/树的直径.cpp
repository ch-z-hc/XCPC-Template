#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 输入编号 1..n 的非空连通树，边权必须非负。返回 {端点 u, 端点 v, 直径长度}。
// 距离累加用 ll。两遍 DFS 共 O(n)，长链递归深度 O(n)。

typedef V < V < pair < int, ll > > > Graph;

pair < int, ll > tree_farthest(int s, const Graph &g) {
    pair < int, ll > ans = {s, 0};
    auto dfs = [&](auto &&self, int u, int fa, ll dis) -> void {
        if (dis > ans.second) ans = {u, dis};
        for (int i = int(g[u].size()) - 1; i >= 0; -- i) {
            auto [v, w] = g[u][i];
            if (v != fa) self(self, v, u, dis + w);
        }
    };
    dfs(dfs, s, 0, 0);
    return ans;
}

// 返回树的两个端点和直径长度。
tuple < int, int, ll > tree_diameter(const Graph &g) {
    if (g.size() <= 1) return {0, 0, 0};
    int u = tree_farthest(1, g).first;
    auto [v, len] = tree_farthest(u, g);
    return {u, v, len};
}

// auto [u, v, len] = tree_diameter(g);
