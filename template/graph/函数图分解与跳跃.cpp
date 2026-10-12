#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

// 函数图：每点恰有一条出边；to 为 1-based，to[0] 不使用。
// build 递归处理反向树；预处理 O(64 n)，空间 O(64 n)，可重复构建。
// jump(u, k) 支持整个 ull 范围，O(64)；distance(u, v) 不可达返回 -1。
// cycles 为 0-based 环列表，沿出边顺序；bel 为环编号，pos 为环上位置。
// 非环点 pos = -1；entry 为入环点，dep 为到环步数；不使用模数。
struct FunctionalGraph {
    int n = 0;
    V < V < int > > g, cycles;
    V < int > bel, pos, entry, dep;
    array < V < int >, 64 > up;

    void dfs(int u) {
        for (int v : g[u]) if (pos[v] == -1) {
            bel[v] = bel[u]; entry[v] = entry[u]; dep[v] = dep[u] + 1;
            dfs(v);
        }
    }

    void build(const V < int > &to) {
        n = int(to.size()) - 1;
        g.assign(n + 1, {}); cycles.clear();
        bel.assign(n + 1, -1); pos.assign(n + 1, -1);
        entry.assign(n + 1, 0); dep.assign(n + 1, 0);
        V < int > deg(n + 1, 0);
        up[0] = to;
        for (int u = 1; u <= n; ++ u) g[to[u]].push_back(u), ++ deg[to[u]];
        queue < int > q;
        for (int u = 1; u <= n; ++ u) if (! deg[u]) q.push(u);
        while (! q.empty()) {
            int u = q.front(); q.pop();
            if (! -- deg[to[u]]) q.push(to[u]);
        }
        for (int u = 1; u <= n; ++ u) if (deg[u] && bel[u] == -1) {
            int id = int(cycles.size()), v = u;
            V < int > ring;
            do {
                bel[v] = id; pos[v] = int(ring.size()); entry[v] = v;
                ring.push_back(v); v = to[v];
            } while (v != u);
            cycles.push_back(ring);
        }
        for (auto &ring : cycles) for (int u : ring) dfs(u);
        for (int k = 1; k < 64; ++ k) {
            up[k].assign(n + 1, 0);
            for (int u = 1; u <= n; ++ u) up[k][u] = up[k - 1][up[k - 1][u]];
        }
    }

    int jump(int u, ull k) {
        for (int i = 0; i < 64; ++ i) if ((k >> i) & 1) u = up[i][u];
        return u;
    }

    ll distance(int u, int v) {
        if (bel[u] != bel[v]) return -1;
        if (dep[v]) {
            if (dep[u] < dep[v]) return -1;
            int d = dep[u] - dep[v];
            return jump(u, d) == v ? d : -1;
        }
        int len = int(cycles[bel[u]].size());
        return ll(dep[u]) + (pos[v] - pos[entry[u]] + len) % len;
    }
};

// FunctionalGraph graph; graph.build({0, 2, 3, 2});
// int v = graph.jump(1, 100); ll d = graph.distance(1, 3);
