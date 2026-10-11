#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 上下界流；顶点编号 1..n，允许自环、重边，要求 0 <= lo <= hi。
// 所有边的 hi 之和须 <= LLONG_MAX / 4；复杂度 O(n^2 m)，空间 O(n + m)。
// solve() 求可行循环流；solve(s, t, mode) 要求 s != t。
// mode 为 0 / 1 / -1 时分别求可行流 / 最大流 / 最小流，可重复调用。
// 返回 {是否可行, 流量}；流量定义为 s 的净流出，允许负值。
// 成功后 f[id] 为原边流量，id 从 0 开始；失败时 f 为空。
// 同时添加两个方向的辅助边，故只存在负流量的可行解也不会漏掉。
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

    void init(int n_) { n = n_; edges.clear(); f.clear(); }

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

// 调用示例：
// LRFlow flow(n);
// int id = flow.add_edge(u, v, lo, hi);
// auto [ok, ans] = flow.solve(s, t, 1);
// if (ok) { ll val = flow.f[id]; }
