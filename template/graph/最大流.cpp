/*
有向图最大流，容量非负，总流量须放入 ll。重复调用 flow 沿用残量，只返回本次新增流量。
一般图 O(n²m)，二分图匹配建成的单位网络 O(m√n)，一般单位容量图 O(m√m)。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct Dinic {
    struct Edge {
        int to;
        ll cap;
    };

    V < Edge > edges;
    V < V < int > > g;
    V < int > dep, cur;
    int n = 0;

    Dinic(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edges.clear();
        g.assign(n + 1, {});
        dep.resize(n + 1);
        cur.resize(n + 1);
    }

    void add_edge(int u, int v, ll cap) {
        int id = int(edges.size());
        edges.push_back({v, cap});
        edges.push_back({u, 0});
        g[u].push_back(id);
        g[v].push_back(id ^ 1);
    }

    bool bfs(int s, int t) {
        fill(dep.begin(), dep.end(), -1);
        queue < int > q;
        q.push(s);
        dep[s] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : g[u]) {
                auto [v, cap] = edges[id];
                if (!cap || dep[v] != -1) continue;
                dep[v] = dep[u] + 1;
                if (v == t) return true;
                q.push(v);
            }
        }
        return dep[t] != -1;
    }

    ll dfs(int u, int t, ll flow) {
        if (u == t) return flow;
        ll left = flow;
        for (int &i = cur[u]; i < int(g[u].size()); i ++) {
            int id = g[u][i];
            auto [v, cap] = edges[id];
            if (!cap || dep[v] != dep[u] + 1) continue;
            ll pushed = dfs(v, t, min(left, cap));
            edges[id].cap -= pushed;
            edges[id ^ 1].cap += pushed;
            left -= pushed;
            if (!left) break;
        }
        return flow - left;
    }

    ll flow(int s, int t) {
        if (s == t) return 0;
        ll ans = 0;
        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);
            ans += dfs(s, t, LLONG_MAX);
        }
        return ans;
    }
};

/*
Dinic dinic; dinic.init(3); dinic.add_edge(1, 2, 4); dinic.add_edge(2, 3, 3);
ll ans = dinic.flow(1, 3);
dinic.add_edge(2, 3, 2); ll extra = dinic.flow(1, 3);
*/
