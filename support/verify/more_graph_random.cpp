#include <bits/stdc++.h>
#include "../../template/graph/点分树.cpp"
#include "../../template/graph/最小斯坦纳树.cpp"

void check_centroid(CentroidTree &tree, const V < V < ll > > &dis,
                    const V < ll > &a, int u, ll radius) {
    ll ans = 0;
    for (int v = 1; v < int(a.size()); ++ v)
        if (dis[u][v] <= radius) ans += a[v];
    assert(tree.query(u, radius) == ans);
}

void test_centroid(mt19937 &rng) {
    CentroidTree tree;
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 30) + 1;
        tree.init(n);
        V < V < pair < int, ll > > > g(n + 1);
        for (int u = 2; u <= n; ++ u) {
            int v = int(rng() % (u - 1)) + 1;
            if (test % 3 == 0) v = u - 1;
            if (test % 3 == 1) v = 1;
            ll w = test % 7 == 0 ? 0 : rng() % 11;
            tree.add_edge(u, v, w);
            g[u].push_back({v, w}); g[v].push_back({u, w});
        }
        V < V < ll > > dis(n + 1, V < ll > (n + 1));
        for (int start = 1; start <= n; ++ start) {
            auto dfs = [&](auto &&self, int u, int fa, ll d) -> void {
                dis[start][u] = d;
                for (auto [v, w] : g[u]) if (v != fa) self(self, v, u, d + w);
            };
            dfs(dfs, start, 0, 0);
        }
        V < ll > a(n + 1);
        tree.build();
        for (int step = 0; step < 100; ++ step) {
            int u = int(rng() % n) + 1;
            ll delta = int(rng() % 201) - 100;
            tree.modify(u, delta); a[u] += delta;
            int v = int(rng() % n) + 1;
            ll radius = int(rng() % 101) - 1;
            if (step % 3 == 0) radius = dis[u][v];
            if (step % 3 == 1) radius = dis[u][v] - 1;
            check_centroid(tree, dis, a, u, radius);
            check_centroid(tree, dis, a, v, 0);
            check_centroid(tree, dis, a, v, -1);
            check_centroid(tree, dis, a, v, numeric_limits < ll > ::max());
        }
        for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v) {
            check_centroid(tree, dis, a, u, dis[u][v]);
            check_centroid(tree, dis, a, u, dis[u][v] - 1);
        }
        tree.build();
        for (int u = 1; u <= n; ++ u) {
            assert(tree.query(u, 0) == 0);
            assert(tree.query(u, numeric_limits < ll > ::max()) == 0);
        }
        tree.modify(1, -17);
        assert(tree.query(n, dis[n][1]) == -17);
        tree.init(1); tree.build();
        assert(tree.query(1, 0) == 0);
        tree.modify(1, 23); assert(tree.query(1, 0) == 23);
    }
    const ll big = 1ll << 60;
    tree.init(4);
    tree.add_edge(1, 2, big); tree.add_edge(2, 3, 0); tree.add_edge(3, 4, big);
    tree.build();
    tree.modify(1, 3); tree.modify(2, -7); tree.modify(3, 11); tree.modify(4, 5);
    assert(tree.query(1, big - 1) == 3);
    assert(tree.query(1, big) == 7 && tree.query(1, big * 2) == 12);
    assert(tree.query(2, 0) == 4 && tree.query(2, big) == 12);
    assert(tree.query(4, numeric_limits < ll > ::max()) == 12);
    assert(tree.query(1, numeric_limits < ll > ::min()) == 0);
    tree.init(0); tree.build();
}

void test_centroid_chain() {
    const int n = 200000;
    CentroidTree tree(n);
    for (int u = 2; u <= n; ++ u) tree.add_edge(u - 1, u);
    tree.build();
    tree.modify(1, 7); tree.modify(n, -5); tree.modify(n / 2, 11);
    assert(tree.query(1, 0) == 7 && tree.query(n, 0) == -5);
    assert(tree.query(n / 2, 0) == 11);
    assert(tree.query(1, n - 1) == 13 && tree.query(n, n - 1) == 13);
    assert(tree.query(1, n / 2 - 2) == 7);
    assert(tree.query(1, n / 2 - 1) == 18);
    tree.modify(1, -7); tree.modify(n, 5); tree.modify(n / 2, -11);
    assert(tree.query(n / 2, n) == 0);
}

optional < ll > brute_steiner(int n, const V < tuple < int, int, ll > > &edges,
                             const V < int > &terminals) {
    if (terminals.empty()) return 0;
    optional < ll > ans;
    for (int mask = 0; mask < (1 << edges.size()); ++ mask) {
        V < int > fa(n + 1);
        iota(fa.begin(), fa.end(), 0);
        auto find = [&](auto &&self, int u) -> int {
            return fa[u] == u ? u : fa[u] = self(self, fa[u]);
        };
        ll sum = 0;
        for (int i = 0; i < int(edges.size()); ++ i) if (mask >> i & 1) {
            auto [u, v, w] = edges[i];
            fa[find(find, u)] = find(find, v); sum += w;
        }
        bool ok = 1;
        for (int u : terminals)
            if (find(find, u) != find(find, terminals[0])) ok = 0;
        if (ok && (! ans || sum < *ans)) ans = sum;
    }
    return ans;
}

void test_steiner(mt19937 &rng) {
    Steiner steiner;
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 7) + 1, m = rng() % 11;
        steiner.init(n);
        V < tuple < int, int, ll > > edges;
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            ll w = rng() % 11;
            steiner.add_edge(u, v, w); edges.push_back({u, v, w});
        }
        V < int > terminals;
        for (int i = 0, k = rng() % (n + 3); i < k; ++ i)
            terminals.push_back(int(rng() % n) + 1);
        ll expected = brute_steiner(n, edges, terminals).value_or(-1);
        assert(steiner.solve(terminals) == expected);
        reverse(terminals.begin(), terminals.end());
        assert(steiner.solve(terminals) == expected);
        assert(steiner.solve({}) == 0);
        assert(steiner.solve({1, 1}) == 0);
        assert(steiner.solve(terminals) == expected);
        steiner.init(2);
        assert(steiner.solve({1, 2}) == -1);
        steiner.add_edge(1, 2, 0);
        assert(steiner.solve({1, 2, 1}) == 0);
    }
    const ll big = 1ll << 60;
    steiner.init(4);
    steiner.add_edge(1, 2, big); steiner.add_edge(2, 3, big);
    steiner.add_edge(1, 3, big * 3); steiner.add_edge(2, 2, 0);
    steiner.add_edge(1, 2, big + 1);
    assert(steiner.solve({1, 3, 1}) == big * 2);
    assert(steiner.solve({1, 2, 3}) == big * 2);
    assert(steiner.solve({1, 4}) == -1);
    steiner.init(2); steiner.add_edge(1, 2, (1ll << 62) - 1);
    assert(steiner.solve({1, 2}) == (1ll << 62) - 1);
    steiner.init(0);
    assert(steiner.solve({}) == 0);
}

int main() {
    mt19937 rng(20261012);
    test_centroid(rng); test_steiner(rng); test_centroid_chain();
    return 0;
}
