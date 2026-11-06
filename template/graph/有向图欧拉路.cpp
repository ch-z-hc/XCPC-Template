#include <bits/stdc++.h>

#define V vector

using namespace std;

// 有向图欧拉路。edges 按输入顺序编号，返回顶点序列；不存在时返回空。
// n >= 1，顶点编号 1-based；无边时返回 {1}。
// 复杂度 O(n + m)，空间 O(n + m)。
// 递归 Hierholzer，递归深度最坏 O(m)。

V < int > euler_d(int n, const V < pair < int, int > > &edges) {
    V < V < int > > g(n + 1);
    V < int > in(n + 1), out(n + 1), cur(n + 1);
    for (int i = 0; i < int(edges.size()); ++ i) {
        auto [u, v] = edges[i]; g[u].push_back(v);
        ++ out[u]; ++ in[v];
    }
    int s = 1, cnt1 = 0, cnt2 = 0;
    for (int u = 1; u <= n; ++ u) {
        if (out[u] - in[u] == 1) { s = u; ++ cnt1; }
        else if (in[u] - out[u] == 1) ++ cnt2;
        else if (in[u] != out[u]) return {};
    }
    if (! ((cnt1 == 1 && cnt2 == 1) || (cnt1 == 0 && cnt2 == 0))) return {};
    if (cnt1 == 0) {
        while (s <= n && g[s].empty()) ++ s;
        if (s > n) return edges.empty() ? V < int >{1} : V < int >{};
    }
    V < int > ans;
    auto dfs = [&](auto &&self, int u) -> void {
        while (cur[u] < int(g[u].size())) self(self, g[u][cur[u] ++]);
        ans.push_back(u);
    };
    dfs(dfs, s);
    if (int(ans.size()) != int(edges.size()) + 1) return {};
    reverse(ans.begin(), ans.end());
    return ans;
}

// 用法：V < int > path = euler_d(n, edges);
