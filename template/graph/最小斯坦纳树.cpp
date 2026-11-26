/*
求非负权无向图连接所有终端的最小代价，空集、单点代价为 0，不可连通返回 -1。
终端去重后 k <= 20，距离和答案须小于 2^62。时间 O(3^k n + 2^k (n + m) log n)，空间 O(2^k n + m)。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct Steiner {
    static const i64 inf = 1ll << 62;
    int n = 0;
    std::vector<std::vector<std::pair<int, i64>>> g;

    Steiner(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    void add_edge(int u, int v, i64 w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    i64 solve(const std::vector<int> &terminals) {
        std::vector<int> a = terminals;
        std::sort(a.begin(), a.end());
        a.erase(std::unique(a.begin(), a.end()), a.end());
        int k = a.size();
        if (k <= 1) return 0;
        int total = 1 << k;
        std::vector<std::vector<i64>> f(total, std::vector<i64>(n + 1, inf));
        for (int i = 0; i < k; i ++) f[1 << i][a[i]] = 0;
        for (int mask = 1; mask < total; mask ++) {
            for (int sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask) {
                int other = mask ^ sub;
                if (sub > other) continue;
                for (int u = 1; u <= n; u ++)
                    if (f[sub][u] < inf - f[other][u])
                        f[mask][u] = std::min(f[mask][u], f[sub][u] + f[other][u]);
            }
            std::priority_queue<std::pair<i64, int>, std::vector<std::pair<i64, int>>,
                                std::greater<std::pair<i64, int>>>
                q;
            for (int u = 1; u <= n; u ++)
                if (f[mask][u] < inf) q.push({f[mask][u], u});
            while (!q.empty()) {
                auto [d, u] = q.top();
                q.pop();
                if (d != f[mask][u]) continue;
                for (auto [v, w] : g[u])
                    if (w < inf - d) {
                        i64 nd = d + w;
                        if (nd < f[mask][v]) {
                            f[mask][v] = nd;
                            q.push({nd, v});
                        }
                    }
            }
        }
        i64 ans = *std::min_element(f.back().begin() + 1, f.back().end());
        return ans == inf ? -1 : ans;
    }
};
const i64 Steiner::inf;

/*
Steiner graph; graph.init(n); graph.add_edge(u, v, w);
i64 ans = graph.solve({1, 3, 5});
*/
