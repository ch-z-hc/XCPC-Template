#include <bits/stdc++.h>
#include "../../template/ds/可撤销并查集.cpp"
#include "../../template/graph/强连通分量.cpp"
#include "../../template/graph/倍增最近公共祖先.cpp"
#include "../../template/graph/欧拉序最近公共祖先.cpp"
#include "../../template/graph/桥.cpp"
#include "../../template/graph/割点.cpp"
#include "../../template/graph/边双连通分量.cpp"
#include "../../template/graph/点双连通分量.cpp"
#include "../../template/graph/二分图匹配.cpp"
#include "../../template/graph/差分约束.cpp"
#include "../../template/graph/有向图欧拉路.cpp"
#include "../../template/graph/无向图欧拉路.cpp"
#include "../../template/graph/换根动态规划与距离和.cpp"
#include "../../template/graph/树的直径.cpp"

int components(int n, const V < pair < int, int > > &edges, int ban_edge = -1, int ban_vertex = 0) {
    V < int > fa(n + 1);
    iota(fa.begin(), fa.end(), 0);
    auto find = [&](auto &&self, int u) -> int {
        return fa[u] == u ? u : fa[u] = self(self, fa[u]);
    };
    for (int i = 0; i < int(edges.size()); ++ i) {
        auto [u, v] = edges[i];
        if (i != ban_edge && u != ban_vertex && v != ban_vertex) fa[find(find, u)] = find(find, v);
    }
    set < int > roots;
    for (int i = 1; i <= n; ++ i) if (i != ban_vertex) roots.insert(find(find, i));
    return int(roots.size());
}

bool brute_euler(int n, const V < pair < int, int > > &edges, bool directed) {
    auto dfs = [&](auto &&self, int u, int mask) -> bool {
        if (mask == (1 << edges.size()) - 1) return 1;
        for (int i = 0; i < int(edges.size()); ++ i) if (! (mask >> i & 1)) {
            auto [a, b] = edges[i];
            if (u == a && self(self, b, mask | (1 << i))) return 1;
            if (! directed && u == b && self(self, a, mask | (1 << i))) return 1;
        }
        return 0;
    };
    for (int u = 1; u <= n; ++ u) if (dfs(dfs, u, 0)) return 1;
    return 0;
}

void check_trail(const V < int > &path, const V < pair < int, int > > &edges, bool directed) {
    assert(path.size() == edges.size() + 1);
    V < int > used(edges.size());
    for (int i = 1; i < int(path.size()); ++ i) {
        bool found = 0;
        for (int j = 0; j < int(edges.size()); ++ j) if (! used[j]) {
            auto [u, v] = edges[j];
            if ((u == path[i - 1] && v == path[i]) ||
                (! directed && v == path[i - 1] && u == path[i])) {
                used[j] = 1; found = 1; break;
            }
        }
        assert(found);
    }
}

int main() {
    mt19937 rng(114514);
    for (int test = 0; test < 300; ++ test) {
        int n = int(rng() % 7) + 1, m = rng() % 12;
        SCC scc(n);
        V < V < int > > reach(n + 1, V < int > (n + 1));
        for (int i = 1; i <= n; ++ i) reach[i][i] = 1;
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            scc.add_edge(u, v); reach[u][v] = 1;
        }
        for (int k = 1; k <= n; ++ k) for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= n; ++ v) reach[u][v] |= reach[u][k] && reach[k][v];
        for (int repeat = 0; repeat < 2; ++ repeat) {
            scc.build();
            for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v)
                assert((scc.bel[u] == scc.bel[v]) == (reach[u][v] && reach[v][u]));
        }
        scc.init(1); assert(scc.build() == 1);

        LCA lca(n); EulerLCA euler(n);
        V < V < int > > g(n + 1);
        for (int i = 2; i <= n; ++ i) {
            int p = int(rng() % (i - 1)) + 1;
            lca.add_edge(i, p); euler.add_edge(i, p);
            g[i].push_back(p); g[p].push_back(i);
        }
        for (int root = 1; root <= n; ++ root) {
            lca.build(root); euler.build(root);
            V < int > fa(n + 1), dep(n + 1), ord{root};
            for (int i = 0; i < int(ord.size()); ++ i)
                for (int v : g[ord[i]]) if (v != fa[ord[i]]) {
                    fa[v] = ord[i]; dep[v] = dep[ord[i]] + 1; ord.push_back(v);
                }
            for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v) {
                int a = u, b = v;
                while (dep[a] > dep[b]) a = fa[a];
                while (dep[b] > dep[a]) b = fa[b];
                while (a != b) { a = fa[a]; b = fa[b]; }
                assert(lca.lca(u, v) == a && euler.lca(u, v) == a);
            }
        }

        {
            Graph tree(n + 1);
            const ll inf = 1ll << 50;
            V < V < ll > > dis(n + 1, V < ll > (n + 1, inf));
            for (int u = 1; u <= n; ++ u) {
                dis[u][u] = 0;
                for (int v : g[u]) if (u < v) {
                    ll w = rng() % 10;
                    tree[u].push_back({v, w}); tree[v].push_back({u, w});
                    dis[u][v] = dis[v][u] = w;
                }
            }
            for (int k = 1; k <= n; ++ k) for (int u = 1; u <= n; ++ u)
                for (int v = 1; v <= n; ++ v) dis[u][v] = min(dis[u][v], dis[u][k] + dis[k][v]);
            auto sum = reroot(tree);
            assert(sum == reroot(tree));
            ll diameter = 0;
            for (int u = 1; u <= n; ++ u) {
                ll expected = 0;
                for (int v = 1; v <= n; ++ v) {
                    expected += dis[u][v];
                    diameter = max(diameter, dis[u][v]);
                }
                assert(sum[u] == expected);
                auto [v, d] = tree_farthest(u, tree);
                assert(d == *max_element(dis[u].begin() + 1, dis[u].end()));
                assert(dis[u][v] == d);
            }
            auto [u, v, len] = tree_diameter(tree);
            assert(len == diameter && dis[u][v] == len);
        }

        V < pair < int, int > > edges;
        Bridge bridge(n); CutVertex cut(n); EBCC ebcc(n); VBCC vbcc(n);
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            if (u == v) continue; // 点双约定不含自环。
            edges.push_back({u, v});
            bridge.add_edge(int(edges.size()), u, v); cut.add_edge(u, v);
            ebcc.add_edge(u, v); vbcc.add_edge(u, v);
        }
        int base = components(n, edges);
        V < int > expected_bridges, expected_cuts;
        for (int i = 0; i < int(edges.size()); ++ i)
            if (components(n, edges, i) > base) expected_bridges.push_back(i + 1);
        for (int u = 1; u <= n; ++ u)
            if (components(n, edges, -1, u) > base) expected_cuts.push_back(u);
        for (int repeat = 0; repeat < 2; ++ repeat) {
            assert(bridge.find() == expected_bridges && cut.find() == expected_cuts);
            ebcc.build(); vbcc.build();
            V < pair < int, int > > kept;
            for (int i = 0; i < int(edges.size()); ++ i) {
                bool is_bridge = binary_search(expected_bridges.begin(), expected_bridges.end(), i + 1);
                assert(bool(ebcc.is_bridge[i]) == is_bridge);
                if (! is_bridge) kept.push_back(edges[i]);
            }
            assert(ebcc.cnt == components(n, kept));
            V < int > vertex_seen(n + 1), edge_seen(edges.size());
            for (int i = 0; i < ebcc.cnt; ++ i) {
                const auto &part = ebcc.bcc[i];
                assert(! part.v.empty());
                for (int u : part.v) {
                    assert(ebcc.bel[u] == i + 1);
                    vertex_seen[u] ++;
                }
                for (int id : part.e) {
                    auto [u, v] = edges[id];
                    assert(! ebcc.is_bridge[id] && ebcc.bel[u] == i + 1 && ebcc.bel[v] == i + 1);
                    edge_seen[id] ++;
                }
            }
            for (int u = 1; u <= n; ++ u) assert(vertex_seen[u] == 1);
            for (int id = 0; id < int(edges.size()); ++ id)
                assert(edge_seen[id] == ! ebcc.is_bridge[id]);
            V < int > seen(edges.size());
            for (const auto &part : vbcc.bcc) {
                set < int > vertices(part.v.begin(), part.v.end()), expected;
                assert(! vertices.empty());
                for (int id : part.e) {
                    seen[id] ++; expected.insert(edges[id].first); expected.insert(edges[id].second);
                }
                if (! part.e.empty()) assert(vertices == expected);
            }
            for (int num : seen) assert(num == 1);
        }

        edges.clear();
        for (int i = 0, count = rng() % 8; i < count; ++ i)
            edges.push_back({int(rng() % n) + 1, int(rng() % n) + 1});
        for (bool directed : {false, true}) {
            auto path = directed ? euler_d(n, edges) : euler_u(n, edges);
            assert(! path.empty() == brute_euler(n, edges, directed));
            if (! path.empty()) check_trail(path, edges, directed);
        }

        int right = int(rng() % 7) + 1;
        Kuhn match(n, right);
        V < V < int > > adj(n + 1);
        for (int u = 1; u <= n; ++ u) for (int v = 1; v <= right; ++ v)
            if (rng() % 2) { match.add_edge(u, v); adj[u].push_back(v); }
        auto brute = [&](auto &&self, int u, int mask) -> int {
            if (u > n) return 0;
            int ans = self(self, u + 1, mask);
            for (int v : adj[u]) if (! (mask >> (v - 1) & 1))
                ans = max(ans, 1 + self(self, u + 1, mask | (1 << (v - 1))));
            return ans;
        };
        int expected = brute(brute, 1, 0);
        assert(match.solve() == expected && match.solve() == expected);

        Diff dc(n);
        const ll inf = 1ll << 50;
        V < V < ll > > dis(n + 1, V < ll > (n + 1, inf));
        for (int i = 1; i <= n; ++ i) dis[i][i] = 0;
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            ll w = int(rng() % 11) - 5;
            dc.add(u, v, w); dis[v][u] = min(dis[v][u], w);
        }
        for (int k = 1; k <= n; ++ k) for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= n; ++ v)
                if (dis[u][k] != inf && dis[k][v] != inf) dis[u][v] = min(dis[u][v], dis[u][k] + dis[k][v]);
        bool feasible = 1;
        for (int u = 1; u <= n; ++ u) if (dis[u][u] < 0) feasible = 0;
        V < ll > val;
        assert(dc.solve(val) == feasible);
        if (feasible) for (auto [u, v, w] : dc.edges) assert(val[u] <= val[v] + w);

        RollbackDSU dsu(n);
        for (int i = 0; i < 30; ++ i) {
            auto fa = dsu.fa, siz = dsu.siz;
            int snap = dsu.snapshot();
            for (int j = 0; j < 10; ++ j) dsu.merge(int(rng() % n) + 1, int(rng() % n) + 1);
            dsu.rollback(snap);
            assert(dsu.fa == fa && dsu.siz == siz);
            dsu.merge(int(rng() % n) + 1, int(rng() % n) + 1);
        }
    }
    mt19937_64 large_rng(20261012);
    for (int test = 0; test < 500; ++ test) {
        int n = 1 + large_rng() % 6;
        Diff dc(n);
        const __int128_t inf = __int128_t(1) << 120;
        V < V < __int128_t > > d(n + 1, V < __int128_t > (n + 1, inf));
        for (int u = 1; u <= n; ++ u) d[u][u] = 0;
        for (int i = 0, m = large_rng() % 25; i < m; ++ i) {
            int u = 1 + large_rng() % n, v = 1 + large_rng() % n;
            // n * m * max(abs(w)) 可装入 ll，保证负环判定前的松弛也不溢出。
            ll w = (ll(large_rng() % 19) - 9) * 100000000000000LL;
            dc.add(u, v, w); d[v][u] = min(d[v][u], __int128_t(w));
        }
        for (int k = 1; k <= n; ++ k) for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= n; ++ v) if (d[u][k] != inf && d[k][v] != inf)
                d[u][v] = min(d[u][v], d[u][k] + d[k][v]);
        bool feasible = true;
        for (int u = 1; u <= n; ++ u) if (d[u][u] < 0) feasible = false;
        V < ll > val;
        assert(dc.solve(val) == feasible);
        if (feasible) for (auto [u, v, w] : dc.edges)
            assert(__int128_t(val[u]) - val[v] <= w);
    }
    return 0;
}
