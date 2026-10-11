#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 常规最小费用最大流；顶点编号 1-based，容量非负。
// 允许负费用边，但残量网络中不能有负环；不处理与源汇无关的负环。
// 首次 SPFA 最坏 O(nm)，之后 O(A m log n)，A 为增广次数；空间 O(n + m)。
// flow 修改残量，重复调用返回本次新增流量与费用；init 清空图。
// 边费不能为 LLONG_MIN；距离、势能和费用使用 __int128_t，最终流量与费用须能用 ll 表示。
// 内部计算不得溢出 __int128_t。

struct MCMF {
    struct Edge {
        int to, rev;
        ll cap, cost;
    };
    int n = 0;
    V < V < Edge > > g;

    MCMF(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    void add_edge(int u, int v, ll cap, ll cost) {
        int a = int(g[u].size()), b = int(g[v].size());
        g[u].push_back({v, b + (u == v), cap, cost});
        g[v].push_back({u, a, 0, -cost});
    }

    pair < ll, ll > flow(int s, int t) {
        if (s == t) return {0, 0};
        typedef __int128_t i128;
        V < i128 > h(n + 1), dis(n + 1);
        V < int > pre(n + 1), pe(n + 1);
        V < char > reach(n + 1);

        V < int > vis(n + 1);
        queue < int > q;
        h[s] = 0;
        reach[s] = 1;
        q.push(s);
        vis[s] = 1;
        while (! q.empty()) {
            int u = q.front();
            q.pop();
            vis[u] = 0;
            for (auto &e : g[u]) {
                if (e.cap && (! reach[e.to] || h[e.to] > h[u] + e.cost)) {
                    h[e.to] = h[u] + e.cost;
                    reach[e.to] = 1;
                    if (! vis[e.to]) {
                        vis[e.to] = 1;
                        q.push(e.to);
                    }
                }
            }
        }
        ll ans_flow = 0;
        i128 ans_cost = 0;
        while (true) {
            fill(reach.begin(), reach.end(), 0);
            fill(pre.begin(), pre.end(), -1);
            priority_queue < pair < i128, int >, V < pair < i128, int > >,
                             greater < pair < i128, int > > > pq;
            dis[s] = 0;
            reach[s] = 1;
            pq.push({0, s});

            while (! pq.empty()) {
                auto [du, u] = pq.top();
                pq.pop();
                if (du != dis[u]) continue;
                for (int i = 0; i < int(g[u].size()); ++ i) {
                    auto &e = g[u][i];
                    if (! e.cap) continue;
                    i128 nd = du + e.cost + h[u] - h[e.to];
                    if (! reach[e.to] || nd < dis[e.to]) {
                        dis[e.to] = nd;
                        reach[e.to] = 1;
                        pre[e.to] = u;
                        pe[e.to] = i;
                        pq.push({nd, e.to});
                    }
                }
            }
            if (! reach[t]) break;

            for (int v = 1; v <= n; ++ v)
                if (reach[v]) h[v] += dis[v];

            ll add = LLONG_MAX;
            for (int v = t; v != s; v = pre[v])
                add = min(add, g[pre[v]][pe[v]].cap);
            for (int v = t; v != s; v = pre[v]) {
                auto &e = g[pre[v]][pe[v]];
                e.cap -= add;
                g[v][e.rev].cap += add;
            }
            ans_flow += add;
            ans_cost += add * (h[t] - h[s]);
        }
        return {ans_flow, static_cast<ll> (ans_cost)};
    }
};

// 调用示例：
// MCMF mcf(n);
// mcf.add_edge(u, v, cap, cost);
// auto [flow, cost] = mcf.flow(s, t);
