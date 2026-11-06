#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 斯坦纳树：无向非负图，顶点 1-based，允许重边、自环和不连通图。
// solve 返回连接全部终端的最小边权和，不恢复边集；不连通返回 -1。
// 终端可重复，去重后 k <= 20，须能承受 O(2^k n) 内存；空集或单点返回 0。
// 时间 O(3^k n + 2^k (n + m) log n)，空间 O(2^k n + n + m)。
// 所有有限答案和距离须小于 inf = 2^62；solve 可重复调用且不修改图。

struct Steiner {
    static const ll inf = 1ll << 62;
    int n = 0;
    V < V < pair < int, ll > > > g;

    Steiner(int n = 0) { init(n); }
    void init(int n_) { n = n_; g.assign(n + 1, {}); }

    void add_edge(int u, int v, ll w) {
        g[u].push_back({v, w}); g[v].push_back({u, w});
    }

    ll solve(const V < int > &terminals) {
        V < int > a = terminals;
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        int k = a.size();
        if (k <= 1) return 0;
        int total = 1 << k;
        V < V < ll > > f(total, V < ll >(n + 1, inf));
        for (int i = 0; i < k; ++ i) f[1 << i][a[i]] = 0;
        for (int mask = 1; mask < total; ++ mask) {
            for (int sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask) {
                int other = mask ^ sub;
                if (sub > other) continue;
                for (int u = 1; u <= n; ++ u)
                    if (f[sub][u] < inf - f[other][u])
                        f[mask][u] = min(f[mask][u], f[sub][u] + f[other][u]);
            }
            priority_queue < pair < ll, int >, V < pair < ll, int > >,
                             greater < pair < ll, int > > > q;
            for (int u = 1; u <= n; ++ u)
                if (f[mask][u] < inf) q.push({f[mask][u], u});
            while (! q.empty()) {
                auto [d, u] = q.top(); q.pop();
                if (d != f[mask][u]) continue;
                for (auto [v, w] : g[u]) if (w < inf - d) {
                    ll nd = d + w;
                    if (nd < f[mask][v]) {
                        f[mask][v] = nd;
                        q.push({nd, v});
                    }
                }
            }
        }
        ll ans = *min_element(f.back().begin() + 1, f.back().end());
        return ans == inf ? -1 : ans;
    }
};
const ll Steiner::inf;
// 用法：Steiner graph(n); graph.add_edge(u, v, w);
// ll ans = graph.solve({1, 3, 5}); // ans != -1 时为最小总边权。
