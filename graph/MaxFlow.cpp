#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge { int to, rev; long long cap; };
    int n = 0;
    vector<vector<Edge>> graph;
    vector<int> level, ptr;
    explicit Dinic(int n = 0) { init(n); }
    void init(int n_) { n = n_; graph.assign(n + 1, {}); level.resize(n + 1); ptr.resize(n + 1); }
    void add_edge(int u, int v, long long cap) {
        Edge a{v, int(graph[v].size()), cap}, b{u, int(graph[u].size()), 0};
        graph[u].push_back(a); graph[v].push_back(b);
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1); queue<int> q;
        level[s] = 0; q.push(s);
        while (!q.empty()) { int u = q.front(); q.pop();
            for (const auto &e : graph[u]) if (e.cap && level[e.to] < 0)
                level[e.to] = level[u] + 1, q.push(e.to);
        }
        return level[t] >= 0;
    }
    long long dfs(int u, int t, long long flow) {
        if (u == t) return flow;
        for (int &i = ptr[u]; i < int(graph[u].size()); ++i) {
            Edge &e = graph[u][i];
            if (e.cap && level[e.to] == level[u] + 1) {
                long long got = dfs(e.to, t, min(flow, e.cap));
                if (got) { e.cap -= got; graph[e.to][e.rev].cap += got; return got; }
            }
        }
        return 0;
    }
    long long max_flow(int s, int t) {
        long long ans = 0, flow;
        while (bfs(s, t)) { fill(ptr.begin(), ptr.end(), 0); while ((flow = dfs(s, t, 1LL << 62))) ans += flow; }
        return ans;
    }
};
