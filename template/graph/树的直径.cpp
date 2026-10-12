#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 连通树的边权必须非负，顶点编号 1-based；两次遍历复杂度 O(n)。
// 所有路径的距离累加须能用 ll 表示，不使用模数。
// 使用递归 DFS，递归深度最坏 O(n)。

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

// 调用示例：
// auto [u, v, len] = tree_diameter(g);
