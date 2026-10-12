#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 容量缩放最小费用最大流：支持负费用、自环以及与源汇不连通的负环（负环流量也计入答案）。
// 顶点编号 1-based，容量非负且小于 2^41；Capacity Scaling 复杂度 O(m^2 log U log m)。
// 最大流量及 (n - 1) * 最大绝对边费须小于 large = 1e12；全部 ll 计算不得溢出。
// flow(s, t) 返回 {最大流量，最大流前提下的最小费用}。

struct ScalingMCMF {
    struct Edge {
        int u, v;
        ll cap, cost;
    };

    V < Edge > edges;
    int n = 0;

    ScalingMCMF(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edges.clear();
    }

    void add_edge(int u, int v, ll cap, ll cost) {
        edges.push_back({u, v, cap, cost});
    }

    pair < ll, ll > flow(int s, int t) {
        if (s == t) return {0, 0};

        // t -> s 的大费用负边把最大流转化为最小费用环流。
        const ll large = 1'000'000'000'000LL;
        const ll inf = 4'000'000'000'000'000'000LL;
        int super = n + 1;

        struct Arc {
            int u, v, next;
            ll cost, raw, cap;
        };

        V < int > head(n + 2, -1), ids;
        V < Arc > e;

        auto add_pair = [&](int u, int v, ll cap, ll cost) {
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

        for (int x = 1; x <= n; ++ x) {
            int id = add_pair(super, x, 0, 0);
            e[id].cap = 1;
        }
        int m = int(e.size());

        V < ll > h(n + 2), dis(n + 2);
        V < int > pre(n + 2);

        auto reduced = [&](int id) {
            return h[e[id].u] + e[id].cost
                 - h[e[id].v];
        };

        auto dijkstra = [&](int s_) {
            fill(dis.begin(), dis.end(), inf);
            fill(pre.begin(), pre.end(), -1);

            priority_queue < pair < ll, int >, V < pair < ll, int > >,
                             greater < pair < ll, int > > > q;
            dis[s_] = 0;
            q.push({0, s_});

            while (! q.empty()) {
                auto [d, x] = q.top();
                q.pop();
                if (d != dis[x]) continue;

                for (int id = head[x]; id != -1; id = e[id].next) {
                    if (e[id].cap == 0) continue;
                    ll nd = d + reduced(id);
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
                ll w = reduced(id);

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

                ll mx = 0;
                for (int x = 1; x <= n; ++ x)
                    if (dis[x] < inf)
                        mx = max(mx, dis[x]);

                for (int x = 1; x <= n; ++ x) {
                    if (dis[x] < inf) {
                        h[x] += dis[x];
                    } else {
                        h[x] += mx + max(0LL, -w);
                    }
                }
            }

            dijkstra(super);
            for (int x = 1; x <= n; ++ x)
                if (dis[x] < inf)
                    h[x] += dis[x];
        };

        // large < 2^40；从高位到低位恢复容量。
        for (int bit = 40; bit >= 0; -- bit) {
            for (int id = 0; id < m; ++ id)
                e[id].cap <<= 1;

            for (int id : ids)
                if ((e[id].raw >> bit) & 1)
                    add_one(id);
            if ((e[special].raw >> bit) & 1)
                add_one(special);
        }

        ll ans = 0;
        for (int id : ids)
            ans += (e[id].raw - e[id].cap) * e[id].cost;

        return {e[special ^ 1].cap, ans};
    }
};

// 调用示例：
// ScalingMCMF mcf(n);
// mcf.add_edge(u, v, cap, cost);
// auto [flow, cost] = mcf.flow(s, t);
