#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// Dinic 最大流；顶点编号 1-based，容量非负。
// 一般图复杂度 O(n^2 m)，空间 O(n + m)；要求容量及流量计算不溢出 ll。
// flow 修改残量网络，再次调用只返回新增流量；init 清空图。
struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int n = 0;
    V < V < Edge > > g;
    V < int > dep, cur;

    Dinic(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dep.resize(n + 1);
        cur.resize(n + 1);
    }

    void add_edge(int u, int v, ll cap) {
        int a = int(g[u].size()), b = int(g[v].size());
        g[u].push_back({v, b + (u == v), cap});
        g[v].push_back({u, a, 0});
    }

    bool bfs(int s, int t) {
        fill(dep.begin(), dep.end(), -1);
        queue < int > q;
        dep[s] = 0;
        q.push(s);
        while (! q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) if (e.cap && dep[e.to] == -1) {
                dep[e.to] = dep[u] + 1;
                q.push(e.to);
            }
        }
        return dep[t] != -1;
    }

    ll dfs(int u, int t, ll flow) {
        if (u == t) return flow;
        for (int &i = cur[u]; i < int(g[u].size()); ++ i) {
            auto &e = g[u][i];
            if (! e.cap || dep[e.to] != dep[u] + 1) continue;
            ll v = dfs(e.to, t, min(flow, e.cap));
            if (! v) continue;
            e.cap -= v;
            g[e.to][e.rev].cap += v;
            return v;
        }
        return 0;
    }

    ll flow(int s, int t) {
        if (s == t) return 0;
        ll ans = 0, flow;
        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);
            while ((flow = dfs(s, t, 1ll << 62))) ans += flow;
        }
        return ans;
    }
};

// 用法：Dinic dinic(n);
// dinic.add_edge(u, v, cap);
// ll ans = dinic.flow(s, t);
