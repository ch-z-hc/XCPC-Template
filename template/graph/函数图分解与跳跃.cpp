/*
每个顶点恰有一条出边，分解环和入环树，支持跳 k 步及有向距离查询。
1-based，k 可取任意 ull。预处理 O(64n)，每点存 64 级后继，两种查询 O(64)。
dis 不可达时返回 -1。entry、dep 分别为入环点、到环步数。
cycles 按出边顺序存环，pos 为环内位置，非环点为 -1。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

struct FuncGraph {
    int n = 0;
    V < V < int > > g, cycles;
    V < int > bel, pos, entry, dep;
    array < V < int >, 64 > up;

    void dfs(int u) {
        for (int v : g[u])
            if (pos[v] == -1) {
                bel[v] = bel[u];
                entry[v] = entry[u];
                dep[v] = dep[u] + 1;
                dfs(v);
            }
    }

    void build(const V < int > &to) {
        n = int(to.size()) - 1;
        g.assign(n + 1, {});
        cycles.clear();
        bel.assign(n + 1, -1);
        pos.assign(n + 1, -1);
        entry.assign(n + 1, 0);
        dep.assign(n + 1, 0);
        V < int > deg(n + 1, 0);
        up[0] = to;
        for (int u = 1; u <= n; u ++) g[to[u]].push_back(u), ++ deg[to[u]];
        queue < int > q;
        for (int u = 1; u <= n; u ++)
            if (!deg[u]) q.push(u);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (!-- deg[to[u]]) q.push(to[u]);
        }
        for (int u = 1; u <= n; u ++)
            if (deg[u] && bel[u] == -1) {
                int id = int(cycles.size()), v = u;
                V < int > ring;
                do {
                    bel[v] = id;
                    pos[v] = int(ring.size());
                    entry[v] = v;
                    ring.push_back(v);
                    v = to[v];
                } while (v != u);
                cycles.push_back(ring);
            }
        for (auto &ring : cycles)
            for (int u : ring) dfs(u);
        for (int k = 1; k < 64; k ++) {
            up[k].assign(n + 1, 0);
            for (int u = 1; u <= n; u ++) up[k][u] = up[k - 1][up[k - 1][u]];
        }
    }

    int jump(int u, ull k) {
        for (int i = 0; i < 64; i ++)
            if ((k >> i) & 1) u = up[i][u];
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

/*
FuncGraph g; g.build({0, 2, 3, 2});
int v = g.jump(1, 100); ll d = g.dis(1, 3);
int entry = g.entry[1], steps = g.dep[1];
auto cycle = g.cycles[g.bel[1]];
*/
