/*
求最小费用最大流，可有负环（含与源汇不连通的负环）和自环。最大容量 U 时 O(m² log U log m)。
容量在 [0, 2^41) 内，最大流量及 (n - 1) * 最大绝对边费须小于 10^12，累计费用须放入 i64。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct ScalingMCMF {
    struct Edge {
        int u, v;
        i64 cap, cost;
    };

    std::vector<Edge> edges;
    int n = 0;

    ScalingMCMF(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edges.clear();
    }

    void add_edge(int u, int v, i64 cap, i64 cost) { edges.push_back({u, v, cap, cost}); }

    std::pair<i64, i64> flow(int s, int t) {
        if (s == t) return {0, 0};

        const i64 large = 1000000000000LL;
        const i64 inf = 4000000000000000000LL;
        int super = n + 1;

        struct Arc {
            int u, v, next;
            i64 cost, raw, cap;
        };

        std::vector<int> head(n + 2, -1), ids;
        std::vector<Arc> e;

        auto add_pair = [&](int u, int v, i64 cap, i64 cost) {
            int id = int(e.size());
            e.push_back({u, v, head[u], cost, cap, 0});
            head[u] = id;
            e.push_back({v, u, head[v], -cost, 0, 0});
            head[v] = id + 1;
            return id;
        };

        for (auto &edge : edges)
            ids.push_back(add_pair(edge.u, edge.v, edge.cap, edge.cost));

        int special = add_pair(t, s, large, -large);

        for (int x = 1; x <= n; x ++) {
            int id = add_pair(super, x, 0, 0);
            e[id].cap = 1;
        }
        int m = int(e.size());

        std::vector<i64> h(n + 2), dis(n + 2);
        std::vector<int> pre(n + 2);

        auto reduced = [&](int id) {
            return h[e[id].u] + e[id].cost - h[e[id].v];
        };

        auto dijkstra = [&](int s_) {
            std::fill(dis.begin(), dis.end(), inf);
            std::fill(pre.begin(), pre.end(), -1);

            std::priority_queue<std::pair<i64, int>, std::vector<std::pair<i64, int>>,
                                std::greater<std::pair<i64, int>>>
                q;
            dis[s_] = 0;
            q.push({0, s_});

            while (!q.empty()) {
                auto [d, x] = q.top();
                q.pop();
                if (d != dis[x]) continue;

                for (int id = head[x]; id != -1; id = e[id].next) {
                    if (e[id].cap == 0) continue;
                    i64 nd = d + reduced(id);
                    if (nd < dis[e[id].v]) {
                        dis[e[id].v] = nd;
                        pre[e[id].v] = id;
                        q.push({nd, e[id].v});
                    }
                }
            }
        };

        auto add_one = [&](int id) {
            int u = e[id].u;
            int v = e[id].v;

            if (e[id].cap != 0) {
                ++ e[id].cap;
            } else {
                dijkstra(v);
                i64 w = reduced(id);

                if (dis[u] < inf && dis[u] + w < 0) {
                    ++ e[id ^ 1].cap;
                    while (u != v) {
                        int pid = pre[u];
                        -- e[pid].cap;
                        ++ e[pid ^ 1].cap;
                        u = e[pid].u;
                    }
                } else {
                    ++ e[id].cap;
                }

                i64 mx = 0;
                for (int x = 1; x <= n; x ++)
                    if (dis[x] < inf) mx = std::max(mx, dis[x]);

                for (int x = 1; x <= n; x ++) {
                    if (dis[x] < inf) {
                        h[x] += dis[x];
                    } else {
                        h[x] += mx + std::max(0LL, -w);
                    }
                }
            }

            dijkstra(super);
            for (int x = 1; x <= n; x ++)
                if (dis[x] < inf) h[x] += dis[x];
        };

        for (int bit = 40; bit >= 0; bit --) {
            for (int id = 0; id < m; id ++) e[id].cap <<= 1;

            for (int id : ids)
                if ((e[id].raw >> bit) & 1) add_one(id);
            if ((e[special].raw >> bit) & 1) add_one(special);
        }

        i64 ans = 0;
        for (int id : ids) ans += (e[id].raw - e[id].cap) * e[id].cost;

        return {e[special ^ 1].cap, ans};
    }
};

/*
ScalingMCMF mcf; mcf.init(n);
mcf.add_edge(u, v, cap, cost);
auto [flow, total_cost] = mcf.flow(s, t);
*/
