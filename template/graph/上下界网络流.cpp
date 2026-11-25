#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
求可行循环流或有源汇的可行流、最大流、最小流，O(n²m)。允许重边、自环，净流量可负。
0 <= lo <= hi，各边 hi 之和 <= LLONG_MAX / 4。有源汇时 s != t。
*/

struct LRFlow {
    struct Edge {
        int u, v;
        ll lo, hi;
    };

    struct Net {
        struct Edge {
            int to, rev;
            ll cap;
        };

        V < V < Edge > > g;
        V < int > dep, cur;

        Net(int n) : g(n + 1), dep(n + 1), cur(n + 1) {}

        int add_edge(int u, int v, ll cap) {
            int a = int(g[u].size()), b = int(g[v].size());
            g[u].push_back({v, b + (u == v), cap});
            g[v].push_back({u, a, 0});
            return a;
        }

        void erase(int u, int id) {
            auto &e = g[u][id];
            g[e.to][e.rev].cap = e.cap = 0;
        }

        bool bfs(int s, int t) {
            fill(dep.begin(), dep.end(), -1);
            queue < int > q;
            dep[s] = 0; q.push(s);
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
                ll val = dfs(e.to, t, min(flow, e.cap));
                if (! val) continue;
                e.cap -= val;
                g[e.to][e.rev].cap += val;
                return val;
            }
            return 0;
        }

        ll flow(int s, int t) {
            ll ans = 0, val;
            while (bfs(s, t)) {
                fill(cur.begin(), cur.end(), 0);
                while ((val = dfs(s, t, 1ll << 62))) ans += val;
            }
            return ans;
        }
    };

    int n;
    V < Edge > edges;
    V < ll > f;

    LRFlow(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edges.clear();
        f.clear();
    }

    int add_edge(int u, int v, ll lo, ll hi) {
        edges.push_back({u, v, lo, hi});
        return int(edges.size()) - 1;
    }

    pair < bool, ll > solve(int s = 0, int t = 0, int mode = 0) {
        int ss = n + 1, tt = n + 2;
        Net net(n + 2);
        V < ll > bal(n + 1);
        V < int > id;
        V < pair < int, int > > extra;
        ll sum = 0, need = 0;
        f.clear();
        for (auto &e : edges) {
            id.push_back(net.add_edge(e.u, e.v, e.hi - e.lo));
            bal[e.u] -= e.lo; bal[e.v] += e.lo;
            sum += e.hi;
        }
        int a = -1, b = -1;
        if (s) {
            a = net.add_edge(t, s, sum);
            b = net.add_edge(s, t, sum);
        }
        for (int u = 1; u <= n; ++ u) {
            if (bal[u] > 0) {
                extra.push_back({ss, net.add_edge(ss, u, bal[u])});
                need += bal[u];
            } else if (bal[u] < 0)
                extra.push_back({u, net.add_edge(u, tt, -bal[u])});
        }
        if (net.flow(ss, tt) != need) return {false, 0};
        ll ans = 0;
        if (s) {
            ans = net.g[s][b].cap - net.g[t][a].cap;
            net.erase(t, a); net.erase(s, b);
        }
        for (auto [u, k] : extra) net.erase(u, k);
        if (s && mode == 1) ans += net.flow(s, t);
        if (s && mode == -1) ans -= net.flow(t, s);
        for (int i = 0; i < int(edges.size()); ++ i) {
            auto &e = edges[i];
            f.push_back(e.hi - net.g[e.u][id[i]].cap);
        }
        return {true, ans};
    }
};

/*
LRFlow g; g.init(2); int id = g.add_edge(1, 2, 1, 3);
auto circulation = g.solve();              // 可行循环流
auto feasible = g.solve(1, 2, 0);          // 可行流
auto maximum = g.solve(1, 2, 1);           // 最大流
auto [ok, value] = g.solve(1, 2, -1);      // 最小流，value 为源点净流出量
if (ok) cout << g.f[id];                   // 最近一次成功求解的边流量
*/
