#include <bits/stdc++.h>
#include "../../template/graph/点双连通分量.cpp"
#include "../../template/graph/边双连通分量.cpp"
#include "../../template/graph/圆方树.cpp"
#include "../../template/graph/边双缩点树.cpp"
#include "../../template/graph/重构树.cpp"
#include "../../template/graph/长链剖分与祖先查询.cpp"
#include "../../template/graph/最大流.cpp"
#include "../../template/graph/最小割树.cpp"

int main() {
    mt19937 rng(20261012);
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 12) + 1, m = int(rng() % 36);
        VBCC vb(n); EBCC eb(n); KruskalTree kr(n);
        V < V < int > > graph(n + 1);
        V < V < ll > > dist(n + 1, V < ll > (n + 1, LLONG_MAX));
        V < V < ll > > widest(n + 1, V < ll > (n + 1, LLONG_MIN));
        for (int u = 1; u <= n; ++ u) dist[u][u] = LLONG_MIN, widest[u][u] = LLONG_MAX;
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            ll w = int(rng() % 31) - 15;
            kr.add_edge(u, v, w);
            if (u != v) {
                vb.add_edge(u, v); eb.add_edge(u, v);
                graph[u].push_back(v); graph[v].push_back(u);
                dist[u][v] = dist[v][u] = min(dist[u][v], w);
                widest[u][v] = widest[v][u] = max(widest[u][v], w);
            }
        }
        for (int k = 1; k <= n; ++ k)
            for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v) {
                dist[u][v] = min(dist[u][v], max(dist[u][k], dist[k][v]));
                widest[u][v] = max(widest[u][v], min(widest[u][k], widest[k][v]));
            }
        for (bool descending : {false, true, false}) {
            kr.build(descending);
            assert(kr.tot == 2 * n - int(kr.roots.size()));
            for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v) {
                ll got;
                ll want = descending ? widest[u][v] : dist[u][v];
                bool connected = descending ? want != LLONG_MIN : want != LLONG_MAX;
                assert(kr.query(u, v, got) == connected);
                if (connected) assert(got == (u == v ? 0 : want));
            }
        }
        vb.build(); eb.build();
        V < V < int > > blocks;
        for (const auto &block : vb.bcc) blocks.push_back(block.v);
        auto block = block_cut_tree(n, blocks);
        auto bridge = bridge_tree(eb.cnt, eb.bel, eb.edges);
        int entries = 0;
        for (int u = 1; u <= eb.cnt; ++ u) for (auto [v, id] : bridge[u]) {
            assert(v != u && eb.is_bridge[id]); entries ++;
        }
        assert(entries == 2 * count(eb.is_bridge.begin(), eb.is_bridge.end(), 1));
        for (int removed = 0; removed <= n; ++ removed) {
            V < int > original(n + 1), forest(block.size());
            auto dfs = [&](auto &&self, int u, int color) -> void {
                original[u] = color;
                for (int v : graph[u])
                    if (v != removed && ! original[v]) self(self, v, color);
            };
            auto dfs_tree = [&](auto &&self, int u, int p, int color) -> void {
                assert(! forest[u]); forest[u] = color;
                for (int v : block[u])
                    if (v != removed && v != p) self(self, v, u, color);
            };
            for (int u = 1; u <= n; ++ u) if (u != removed) {
                if (! original[u]) dfs(dfs, u, u);
                if (! forest[u]) dfs_tree(dfs_tree, u, 0, u);
            }
            for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v)
                if (u != removed && v != removed)
                    assert((original[u] == original[v]) == (forest[u] == forest[v]));
        }
        for (int start = 1; start <= n; ++ start) {
            V < int > reach(eb.cnt + 1); reach[eb.bel[start]] = 1;
            auto dfs = [&](auto &&self, int u, int p) -> void {
                for (auto [v, id] : bridge[u]) if (v != p) {
                    assert(! reach[v]); reach[v] = reach[u] + 1; self(self, v, u);
                }
            };
            dfs(dfs, eb.bel[start], 0);
            for (int v = 1; v <= n; ++ v)
                assert(bool(reach[eb.bel[v]]) == (dist[start][v] != LLONG_MAX));
        }
    }
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 80) + 1; KthAncestor tree(n);
        for (int u = 2; u <= n; ++ u) tree.add_edge(u, int(rng() % (u - 1)) + 1);
        for (int repeat = 0; repeat < 2; ++ repeat) {
            int root = int(rng() % n) + 1;
            tree.build(root);
            V < int > parent(n + 1, -1); parent[root] = 0;
            queue < int > q; q.push(root);
            while (! q.empty()) {
                int u = q.front(); q.pop();
                for (int v : tree.g[u]) if (parent[v] == -1) parent[v] = u, q.push(v);
            }
            for (int u = 1; u <= n; ++ u) {
                int p = u;
                for (int k = 0; k <= n + 1; ++ k) {
                    assert(tree.query(u, k) == p); p = parent[p] < 0 ? 0 : parent[p];
                }
            }
        }
    }
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 7) + 1, m = int(rng() % 25); GomoryHu tree(n);
        for (int i = 0; i < m; ++ i)
            tree.add_edge(int(rng() % n) + 1, int(rng() % n) + 1, rng() % 11);
        tree.build(); auto saved = tree.t;
        for (int s = 1; s <= n; ++ s) for (int t = s + 1; t <= n; ++ t) {
            ll expected = LLONG_MAX;
            for (int mask = 0; mask < (1 << n); ++ mask)
                if ((mask >> (s - 1) & 1) && ! (mask >> (t - 1) & 1)) {
                    ll value = 0;
                    for (auto [u, v, cap] : tree.edges)
                        if (((mask >> (u - 1)) ^ (mask >> (v - 1))) & 1) value += cap;
                    expected = min(expected, value);
                }
            assert(tree.query(s, t) == expected && tree.query(t, s) == expected);
        }
        for (int u = 1; u <= n; ++ u) assert(tree.query(u, u) == 0);
        tree.build(); assert(tree.t == saved);
    }
    {
        GomoryHu tree(2); tree.add_edge(1, 2, LLONG_MAX);
        tree.build(); assert(tree.query(1, 2) == LLONG_MAX);
        tree.init(3); tree.build(); assert(tree.query(1, 3) == 0);
    }
    cout << "advanced trees: Kruskal / block-cut / bridge / ancestor / Gomory-Hu OK\n";
}
