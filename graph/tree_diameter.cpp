#include <bits/stdc++.h>
using namespace std;

using WeightedTree = vector<vector<pair<int, long long>>>;

pair<int, long long> tree_farthest(int start, const WeightedTree &g) {
    pair<int, long long> best = {start, 0};
    vector<tuple<int, int, long long>> st;
    st.push_back({start, 0, 0});
    while (!st.empty()) {
        auto [u, parent, dist] = st.back();
        st.pop_back();
        if (dist > best.second) best = {u, dist};
        for (auto [v, w] : g[u])
            if (v != parent) st.push_back({v, u, dist + w});
    }
    return best;
}

// 返回树的两个端点和直径长度；默认下标 1-based、边权非负。
tuple<int, int, long long> tree_diameter(const WeightedTree &g) {
    if (g.size() <= 1) return {0, 0, 0};
    int u = tree_farthest(1, g).first;
    auto [v, diameter] = tree_farthest(u, g);
    return {u, v, diameter};
}

// Usage:
// auto [u, v, length] = tree_diameter(g);
