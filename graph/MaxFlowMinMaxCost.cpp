#include <bits/stdc++.h>
using namespace std;

// 最小费用最大流；容量非负、顶点编号 1-based，要求不存在可达负环。
// 使用 SPFA 求最短增广路，最坏复杂度 O(flow * V * E)。
struct MinCostMaxFlow {
    struct Edge { int to, rev; long long cap, cost; };
    int n = 0;
    vector<vector<Edge>> graph;
    explicit MinCostMaxFlow(int n = 0) { init(n); }
    void init(int n_) { n = n_; graph.assign(n + 1, {}); }
    void add_edge(int u, int v, long long cap, long long cost) {
        Edge a{v, int(graph[v].size()), cap, cost}, b{u, int(graph[u].size()), 0, -cost};
        graph[u].push_back(a); graph[v].push_back(b);
    }
    pair<long long, long long> flow(int s, int t) {
        if (s == t) return {0, 0};
        long long total_flow = 0, total_cost = 0;
        const long long INF = 1LL << 62;
        while (true) {
            vector<long long> dist(n + 1, INF); vector<int> pv(n + 1), pe(n + 1), inq(n + 1);
            queue<int> q; dist[s] = 0; q.push(s); inq[s] = 1;
            while (!q.empty()) { int u = q.front(); q.pop(); inq[u] = 0;
                for (int i = 0; i < int(graph[u].size()); ++i) { auto &e = graph[u][i];
                    if (e.cap && dist[e.to] > dist[u] + e.cost) {
                        dist[e.to] = dist[u] + e.cost; pv[e.to] = u; pe[e.to] = i;
                        if (!inq[e.to]) inq[e.to] = 1, q.push(e.to);
                    }
                }
            }
            if (dist[t] == INF) break;
            long long add = INF;
            for (int v = t; v != s; v = pv[v]) add = min(add, graph[pv[v]][pe[v]].cap);
            for (int v = t; v != s; v = pv[v]) { auto &e = graph[pv[v]][pe[v]]; e.cap -= add; graph[v][e.rev].cap += add; }
            total_flow += add; total_cost += add * dist[t];
        }
        return {total_flow, total_cost};
    }
};
