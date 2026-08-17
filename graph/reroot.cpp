#include <bits/stdc++.h>
using namespace std;

// 换根 DP 示例：求树上每个点到所有点的距离和。
// 输入必须是连通树，顶点编号 1-based，边权可为任意整数；复杂度 O(n)。
using RerootTree = vector<vector<pair<int, long long>>>;

vector<long long> tree_distance_sums(const RerootTree &g) {
    int n = int(g.size()) - 1;
    if (n <= 0) return {};
    vector<int> parent(n + 1), order;
    vector<long long> weight_to_parent(n + 1), sub_size(n + 1, 1), down(n + 1);
    order.reserve(n);
    order.push_back(1);
    for (int i = 0; i < int(order.size()); ++i) {
        int u = order[i];
        for (auto [v, w] : g[u]) if (v != parent[u]) {
            parent[v] = u;
            weight_to_parent[v] = w;
            order.push_back(v);
        }
    }
    for (int i = n - 1; i > 0; --i) {
        int u = order[i], p = parent[u];
        sub_size[p] += sub_size[u];
        down[p] += down[u] + sub_size[u] * weight_to_parent[u];
    }

    vector<long long> answer(n + 1);
    answer[1] = down[1];
    for (int u : order) for (auto [v, w] : g[u]) if (parent[v] == u)
        answer[v] = answer[u] + (static_cast<long long>(n) - 2 * sub_size[v]) * w;
    return answer;
}

// Usage:
// vector<long long> sum = tree_distance_sums(g);
