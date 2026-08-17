#include <bits/stdc++.h>
using namespace std;

// 差分约束：add_constraint(u, v, w) 表示 x[u] - x[v] <= w。
struct DifferenceConstraints {
    using ll = long long;
    int n = 0;
    vector<tuple<int, int, ll>> edges;

    explicit DifferenceConstraints(int n = 0) : n(n) {}
    void init(int n_) { n = n_; edges.clear(); }
    void add_constraint(int u, int v, ll w) { edges.push_back({u, v, w}); }

    // 返回 false 表示存在负环（无解），否则 dist 是一组可行解。
    bool solve(vector<ll> &dist) const {
        dist.assign(n + 1, 0);
        for (int round = 1; round <= n; ++round) {
            bool changed = false;
            for (auto [u, v, w] : edges) {
                if (dist[u] > dist[v] + w) {
                    dist[u] = dist[v] + w;
                    changed = true;
                }
            }
            if (!changed) return true;
            if (round == n) return false;
        }
        return true;
    }
};
