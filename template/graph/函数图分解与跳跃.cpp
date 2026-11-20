#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

// to[u] 是点 u 唯一的后继，点编号 1..n。jump(u, k) 沿出边走 k 步，k 可取任意 ull。
// dis(u, v) 返回从 u 到 v 的步数，不可达为 -1。entry、dep 分别是入环点和到环的步数。
// cycles 按出边顺序列环，bel 是从 0 开始的环号，pos 是环内下标，非环点为 -1。
// build 可重建，反向树用递归。建表时间和空间 O(64n)，跳跃或查距离 O(64)。
struct FuncGraph {
    int n = 0;
    V < V < int > > g, cycles;
    V < int > bel, pos, entry, dep;
    array < V < int >, 64 > up;

    void dfs(int u) {
        for (int v : g[u]) if (pos[v] == -1) {
            bel[v] = bel[u];
            entry[v] = entry[u];
            dep[v] = dep[u] + 1;
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
                bel[v] = id;
                pos[v] = int(ring.size());
                entry[v] = v;
                ring.push_back(v); v = to[v];
            } while (v != u);
            cycles.push_back(ring);
        }
        for (auto &ring : cycles)
            for (int u : ring) dfs(u);
        for (int k = 1; k < 64; ++ k) {
            up[k].assign(n + 1, 0);
            for (int u = 1; u <= n; ++ u) up[k][u] = up[k - 1][up[k - 1][u]];
        }
    }

    int jump(int u, ull k) {
        for (int i = 0; i < 64; ++ i) if ((k >> i) & 1) u = up[i][u];
        return u;
    }

    ll dis(int u, int v) {
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

// FuncGraph graph; graph.build({0, 2, 3, 2});
// int v = graph.jump(1, 100); ll d = graph.dis(1, 3);
