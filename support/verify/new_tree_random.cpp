#include <cassert>
#include "../../template/graph/HLD.cpp"
#include "../../template/graph/VirtualTree.cpp"
#include "../../template/graph/Centroid.cpp"
#include "../../template/graph/TwoSAT.cpp"

int brute_lca(int u, int v, const V < int > &fa, const V < int > &dep) {
    while (dep[u] > dep[v]) u = fa[u];
    while (dep[v] > dep[u]) v = fa[v];
    while (u != v) u = fa[u], v = fa[v];
    return u;
}

bool brute_ancestor(int u, int v, const V < int > &fa) {
    while (v && v != u) v = fa[v];
    return v == u;
}

void test_tree(mt19937 &rng) {
    HLD hld;
    VTree tree;
    for (int test = 0; test < 400; ++ test) {
        int n = int(rng() % 35) + 1;
        hld.init(n); tree.init(n);
        for (int v = 2; v <= n; ++ v) {
            int u = int(rng() % (v - 1)) + 1;
            hld.add_edge(u, v); tree.add_edge(u, v);
        }
        for (int rep = 0; rep < 3; ++ rep) {
            int root = int(rng() % n) + 1;
            hld.build(root); tree.build(root);
            assert(tree.nodes.empty());
            for (const auto &adj : tree.t) assert(adj.empty());
            V < int > fa(n + 1), dep(n + 1);
            auto dfs = [&](auto &&self, int u, int p) -> void {
                fa[u] = p;
                for (int v : hld.g[u]) if (v != p) {
                    dep[v] = dep[u] + 1;
                    self(self, v, u);
                }
            };
            dfs(dfs, root, 0);
            assert(hld.tim == n && tree.tim == n);
            V < int > seen(n + 1);
            for (int u = 1; u <= n; ++ u) {
                assert(hld.fa[u] == fa[u] && hld.dep[u] == dep[u]);
                assert(hld.in[u] >= 1 && hld.in[u] <= n);
                assert(hld.rev[hld.in[u]] == u);
                assert(! seen[hld.in[u]] ++);
                int siz = 0;
                for (int v = 1; v <= n; ++ v) {
                    bool yes = brute_ancestor(u, v, fa);
                    assert(tree.ancestor(u, v) == yes);
                    assert((hld.in[u] <= hld.in[v] && hld.in[v] <= hld.out[u])
                           == yes);
                    siz += yes;
                }
                assert(hld.siz[u] == siz);
                assert(hld.out[u] - hld.in[u] + 1 == siz);
            }
            for (int q = 0; q < 45; ++ q) {
                int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
                int lca = brute_lca(u, v, fa, dep);
                assert(hld.lca(u, v) == lca && tree.lca(u, v) == lca);
                V < int > want(n + 1), got(n + 1);
                for (int x = u; x != lca; x = fa[x]) want[x] ++;
                for (int x = v; x != lca; x = fa[x]) want[x] ++;
                want[lca] ++;
                hld.path(u, v, [&](int l, int r) {
                    assert(l >= 1 && l <= r && r <= n);
                    for (int i = l; i <= r; ++ i) got[hld.rev[i]] ++;
                });
                assert(got == want);
            }
            for (int q = 0; q < 25; ++ q) {
                int k = rng() % 13;
                V < int > key;
                for (int i = 0; i < k; ++ i) key.push_back(int(rng() % n) + 1);
                set < int > want(key.begin(), key.end());
                for (int u : key) for (int v : key)
                    want.insert(brute_lca(u, v, fa, dep));
                int rt = tree.query(key);
                set < int > got(tree.nodes.begin(), tree.nodes.end());
                assert(got == want && got.size() == tree.nodes.size());
                if (key.empty()) { assert(! rt); }
                else { assert(got.count(rt)); }
                V < int > indeg(n + 1);
                int edges = 0;
                for (int u = 1; u <= n; ++ u) {
                    if (! got.count(u)) assert(tree.t[u].empty());
                    for (auto [v, w] : tree.t[u]) {
                        assert(got.count(u) && got.count(v) && u != v);
                        assert(brute_ancestor(u, v, fa));
                        assert(w == dep[v] - dep[u]);
                        for (int x : got) if (x != u && x != v)
                            assert(! (brute_ancestor(u, x, fa)
                                      && brute_ancestor(x, v, fa)));
                        indeg[v] ++; edges ++;
                    }
                }
                assert(edges == max(0, int(got.size()) - 1));
                for (int u : got) {
                    assert(indeg[u] == (u != rt));
                    assert(brute_ancestor(rt, u, fa));
                }
                V < int > reached(n + 1);
                auto walk = [&](auto &&self, int u) -> void {
                    assert(! reached[u] ++);
                    for (auto [v, w] : tree.t[u]) self(self, v);
                };
                if (rt) walk(walk, rt);
                for (int u : got) assert(reached[u] == 1);
            }
        }
    }
}

void test_centroid(mt19937 &rng) {
    Centroid tree;
    assert(tree.query(0) == 0);
    for (int test = 0; test < 600; ++ test) {
        int n = int(rng() % 35) + 1;
        tree.init(n);
        for (int v = 2; v <= n; ++ v)
            tree.add_edge(int(rng() % (v - 1)) + 1, v, rng() % 6);
        V < ll > dis;
        for (int s = 1; s <= n; ++ s) {
            auto dfs = [&](auto &&self, int u, int p, ll d) -> void {
                if (u > s) dis.push_back(d);
                for (auto [v, w] : tree.g[u]) if (v != p)
                    self(self, v, u, d + w);
            };
            dfs(dfs, s, 0, 0);
        }
        for (ll k : {-1LL, 0LL, 1LL, 5LL, 17LL, 200LL, LLONG_MAX}) {
            ll ans = count_if(dis.begin(), dis.end(), [&](ll d) { return d <= k; });
            assert(tree.query(k) == ans);
            assert(tree.query(k) == ans);
        }
    }
    tree.init(3);
    tree.add_edge(1, 2, LLONG_MAX); tree.add_edge(2, 3, 0);
    assert(tree.query(LLONG_MAX) == 3);
    assert(tree.query(0) == 1);
    tree.init(8);
    for (int v = 2; v <= 8; ++ v) tree.add_edge(1, v, 0);
    assert(tree.query(0) == 28);
}

void test_sat(mt19937 &rng) {
    SAT sat;
    for (int test = 0; test < 3500; ++ test) {
        int n = rng() % 9;
        sat.init(n);
        V < array < int, 4 > > clauses;
        int m = n ? rng() % 30 : 0;
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            int x = rng() % 2, y = rng() % 2;
            if (rng() % 5 == 0) v = u, y = x, sat.force(u, x);
            else sat.add(u, x, v, y);
            clauses.push_back({u, x, v, y});
        }
        bool ok = false;
        for (int mask = 0; mask < (1 << n); ++ mask) {
            bool yes = true;
            for (auto [u, x, v, y] : clauses)
                yes &= ((mask >> (u - 1) & 1) == x)
                       || ((mask >> (v - 1) & 1) == y);
            ok |= yes;
        }
        for (int rep = 0; rep < 2; ++ rep) {
            assert(sat.solve() == ok);
            if (! ok) { assert(sat.ans.empty()); continue; }
            assert(int(sat.ans.size()) == n + 1);
            for (int u = 1; u <= n; ++ u) assert(sat.ans[u] <= 1 && sat.ans[u] >= 0);
            for (auto [u, x, v, y] : clauses)
                assert(sat.ans[u] == x || sat.ans[v] == y);
        }
    }
    sat.init(1); sat.force(1, 0);
    assert(sat.solve() && sat.ans[1] == 0);
    sat.force(1, 1);
    assert(! sat.solve());
}

void test_deep() {
    int n = 200000;
    {
        HLD hld(n);
        for (int u = 1; u < n; ++ u) hld.add_edge(u, u + 1);
        hld.build();
        assert(hld.lca(n, n / 2) == n / 2);
        int cnt = 0;
        hld.path(1, n, [&](int l, int r) { cnt += r - l + 1; });
        assert(cnt == n && hld.siz[1] == n);
        hld.build(n);
        assert(hld.lca(1, n / 2) == n / 2 && hld.siz[n] == n);
    }
    {
        VTree tree(n);
        for (int u = 1; u < n; ++ u) tree.add_edge(u, u + 1);
        tree.build();
        assert(tree.query({1, n}) == 1);
        assert(tree.t[1] == (V < pair < int, int > > {{n, n - 1}}));
        tree.build(n);
        assert(tree.query({1, n}) == n && tree.t[1].empty());
        assert(tree.t[n] == (V < pair < int, int > > {{1, n - 1}}));
        assert(tree.query({}) == 0 && tree.t[n].empty());
    }
    {
        // 每个变量有两个蕴含图点，100000 个变量对应 200000 个图点。
        int vars = n / 2;
        SAT sat(vars);
        for (int u = 1; u < vars; ++ u) sat.add(u, 0, u + 1, 1);
        sat.force(1, 1);
        assert(sat.solve());
        for (int u = 1; u <= vars; ++ u) assert(sat.ans[u] == 1);
        sat.force(vars, 0);
        assert(! sat.solve());
    }
    {
        Centroid tree(n);
        for (int u = 1; u < n; ++ u) tree.add_edge(u, u + 1, 1);
        assert(tree.query(0) == 0);
        assert(tree.query(1) == n - 1);
    }
}

int main() {
    mt19937 rng(20261012);
    test_tree(rng);
    test_centroid(rng);
    test_sat(rng);
    test_deep();
    cout << "PASS: HLD / VTree / Centroid / SAT random and deep tests\n";
}
