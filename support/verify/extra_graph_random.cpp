#include <bits/stdc++.h>
#include <cassert>

#include "../../template/graph/DirectedMST.cpp"
#include "../../template/graph/GeneralMatching.cpp"
#include "../../template/graph/Johnson.cpp"
#include "../../template/graph/GlobalMinCut.cpp"
#include "../../template/graph/ZeroOneBFS.cpp"
#include "../../template/graph/Floyd.cpp"
#include "../../template/graph/HopcroftKarp.cpp"

using namespace std;

mt19937 rng(20261010);

int rnd(int l, int r) {
    return uniform_int_distribution < int > (l, r)(rng);
}

optional < ll > brute_dmst(const DMST &g, int root) {
    V < V < int > > in(g.n + 1);
    for (int i = 0; i < int(g.edges.size()); ++ i) {
        auto e = g.edges[i];
        if (e.u != e.v && e.v != root) in[e.v].push_back(i);
    }
    V < int > fa(g.n + 1);
    optional < ll > ans;
    auto enumerate = [&](auto &&self, int v, ll sum) -> void {
        if (v == g.n + 1) {
            V < int > vis(g.n + 1);
            auto dfs = [&](auto &&walk, int u) -> void {
                vis[u] = 1;
                for (int x = 1; x <= g.n; ++ x)
                    if (fa[x] == u && ! vis[x]) walk(walk, x);
            };
            dfs(dfs, root);
            for (int u = 1; u <= g.n; ++ u) if (! vis[u]) return;
            if (! ans || sum < *ans) ans = sum;
            return;
        }
        if (v == root) { self(self, v + 1, sum); return; }
        for (int i : in[v]) {
            auto e = g.edges[i];
            fa[v] = e.u;
            self(self, v + 1, sum + e.w);
        }
    };
    enumerate(enumerate, 1, 0);
    return ans;
}

void check_dmst() {
    DMST g(1);
    assert(g.solve(1) == optional < ll > (0));
    g.add_edge(1, 1, -10);
    assert(g.solve(1) == optional < ll > (0));
    g.init(2);
    assert(! g.solve(1));
    g.add_edge(1, 2, LLONG_MAX);
    assert(g.solve(1) == optional < ll > (LLONG_MAX));
    g.init(3);
    g.add_edge(1, 2, 10); g.add_edge(1, 3, 12);
    g.add_edge(2, 3, -5); g.add_edge(3, 2, -4);
    assert(g.solve(1) == optional < ll > (5));
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(1, 5), root = rnd(1, n);
        g.init(n);
        for (int v = 1; v <= n; ++ v) {
            int cnt = rnd(0, 3);
            while (cnt --) g.add_edge(rnd(1, n), v, rnd(-20, 20));
        }
        auto ans = brute_dmst(g, root);
        assert(g.solve(root) == ans);
        assert(g.solve(root) == ans);
        for (int r = 1; r <= n; ++ r)
            assert(g.solve(r) == brute_dmst(g, r));
    }
}

int brute_matching(const V < V < char > > &g) {
    int n = int(g.size()) - 1;
    V < int > dp(1 << n, -1);
    auto dfs = [&](auto &&self, int mask) -> int {
        if (! mask) return 0;
        int &ans = dp[mask];
        if (ans != -1) return ans;
        int u = __builtin_ctz(unsigned(mask)), rest = mask ^ (1 << u);
        ans = self(self, rest);
        for (int v = u + 1; v < n; ++ v)
            if ((rest >> v & 1) && g[u + 1][v + 1])
                ans = max(ans, 1 + self(self, rest ^ (1 << v)));
        return ans;
    };
    return dfs(dfs, (1 << n) - 1);
}

void check_match_plan(const Blossom &g, int ans) {
    int cnt = 0;
    for (int u = 1; u <= g.n; ++ u) {
        int v = g.match[u];
        assert(v >= 0 && v <= g.n);
        if (! v) continue;
        assert(v != u && g.g[u][v] && g.match[v] == u);
        cnt ++;
    }
    assert(cnt == 2 * ans);
}

void check_blossom() {
    Blossom g;
    assert(g.solve() == 0);
    g.init(1); g.add_edge(1, 1);
    assert(g.solve() == 0);
    g.init(3);
    g.add_edge(1, 2); g.add_edge(2, 3); g.add_edge(3, 1);
    assert(g.solve() == 1);
    check_match_plan(g, 1);
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 12), density = rnd(0, 100);
        g.init(n);
        for (int u = 1; u <= n; ++ u)
            for (int v = u; v <= n; ++ v) if (rnd(0, 99) < density) {
                g.add_edge(u, v);
                if (rnd(0, 1)) g.add_edge(u, v);
            }
        int ans = brute_matching(g.g);
        assert(g.solve() == ans);
        check_match_plan(g, ans);
        assert(g.solve() == ans);
        check_match_plan(g, ans);
    }
}

void check_shortest_paths() {
    typedef __int128_t i128;
    const i128 inf = i128(1) << 100;
    Johnson g;
    assert(g.solve().has_value());
    g.init(1); g.add_edge(1, 1, -1);
    assert(! g.solve());
    g.init(4); g.add_edge(3, 4, -2); g.add_edge(4, 3, 1);
    assert(! g.solve()); // 从顶点 1 不可达的负环也必须检出。
    V < V < ll > > cycle(5, V < ll > (5, Johnson::inf));
    for (int i = 1; i <= 4; ++ i) cycle[i][i] = 0;
    cycle[3][4] = -2; cycle[4][3] = 1;
    assert(! floyd(cycle));
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 9);
        g.init(n);
        V < V < i128 > > truth(n + 1, V < i128 > (n + 1, inf));
        V < V < ll > > dis(n + 1, V < ll > (n + 1, Johnson::inf));
        V < int > h(n + 1);
        for (int i = 1; i <= n; ++ i) h[i] = rnd(-30, 30);
        for (int i = 1; i <= n; ++ i) truth[i][i] = dis[i][i] = 0;
        int m = n ? rnd(0, n * n * 2) : 0;
        for (int i = 0; i < m; ++ i) {
            int u = rnd(1, n), v = rnd(1, n), w;
            // 势能造负边无负环图、任意负权图、非负图各占约三分之一。
            if (t % 3 == 0) w = rnd(0, 30) + h[v] - h[u];
            else if (t % 3 == 1) w = rnd(-10, 20);
            else w = rnd(0, 30);
            g.add_edge(u, v, w);
            truth[u][v] = min(truth[u][v], i128(w));
            dis[u][v] = min(dis[u][v], ll(w));
        }
        for (int k = 1; k <= n; ++ k)
            for (int i = 1; i <= n; ++ i) if (truth[i][k] != inf)
                for (int j = 1; j <= n; ++ j) if (truth[k][j] != inf)
                    truth[i][j] = min(truth[i][j], truth[i][k] + truth[k][j]);
        bool ok = true;
        for (int i = 1; i <= n; ++ i) if (truth[i][i] < 0) ok = false;
        auto ans = g.solve();
        assert(ans.has_value() == ok);
        assert(g.solve() == ans);
        assert(floyd(dis) == ok);
        if (! ok) continue;
        for (int i = 1; i <= n; ++ i)
            for (int j = 1; j <= n; ++ j) {
                ll want = truth[i][j] == inf ? Johnson::inf : ll(truth[i][j]);
                assert((*ans)[i][j] == want && dis[i][j] == want);
            }
        auto old = dis;
        assert(floyd(dis) && dis == old);
    }
}

ll brute_cut(const MinCut &g) {
    if (g.n < 2) return 0;
    ll ans = LLONG_MAX;
    for (int mask = 1; mask < (1 << g.n) - 1; ++ mask) {
        ll sum = 0;
        for (int u = 1; u <= g.n; ++ u)
            for (int v = u + 1; v <= g.n; ++ v)
                if (((mask >> (u - 1)) ^ (mask >> (v - 1))) & 1)
                    sum += g.g[u][v];
        ans = min(ans, sum);
    }
    return ans;
}

void check_cut_plan(const MinCut &g, const pair < ll, V < int > > &ans) {
    if (g.n < 2) {
        assert(ans.first == 0 && ans.second.empty());
        return;
    }
    assert(! ans.second.empty() && int(ans.second.size()) < g.n);
    V < int > side(g.n + 1);
    for (int u : ans.second) {
        assert(u >= 1 && u <= g.n && ! side[u]);
        side[u] = 1;
    }
    ll sum = 0;
    for (int u = 1; u <= g.n; ++ u)
        for (int v = u + 1; v <= g.n; ++ v)
            if (side[u] != side[v]) sum += g.g[u][v];
    assert(sum == ans.first);
}

void check_mincut() {
    MinCut g;
    check_cut_plan(g, g.solve());
    g.init(1); g.add_edge(1, 1, 100);
    check_cut_plan(g, g.solve());
    g.init(2); g.add_edge(1, 2, LLONG_MAX - 1);
    assert(g.solve().first == LLONG_MAX - 1);
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 9);
        g.init(n);
        int m = n ? rnd(0, n * n * 2) : 0;
        while (m --) g.add_edge(rnd(1, n), rnd(1, n), rnd(0, 20));
        auto ans = g.solve();
        assert(ans.first == brute_cut(g));
        check_cut_plan(g, ans);
        assert(g.solve() == ans);
    }
}

void check_bfs01() {
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(1, 10), s = rnd(1, n);
        V < V < pair < int, int > > > g(n + 1);
        int m = rnd(0, n * n * 2);
        while (m --) g[rnd(1, n)].push_back({rnd(1, n), rnd(0, 1)});
        V < int > dis(n + 1, INT_MAX / 2);
        dis[s] = 0;
        for (int k = 1; k < n; ++ k)
            for (int u = 1; u <= n; ++ u) if (dis[u] != INT_MAX / 2)
                for (auto [v, w] : g[u]) dis[v] = min(dis[v], dis[u] + w);
        assert(bfs01(g, s) == dis);
        assert(bfs01(g, s) == dis);
    }
}

int brute_bipartite(const HK &g) {
    V < V < int > > dp(g.n + 2, V < int > (1 << g.m, -1));
    auto dfs = [&](auto &&self, int u, int mask) -> int {
        if (u > g.n) return 0;
        int &ans = dp[u][mask];
        if (ans != -1) return ans;
        ans = self(self, u + 1, mask);
        for (int v : g.g[u]) if (! (mask >> (v - 1) & 1))
            ans = max(ans, 1 + self(self, u + 1, mask | (1 << (v - 1))));
        return ans;
    };
    return dfs(dfs, 1, 0);
}

void check_cover_plan(const HK &g, int ans) {
    auto [a, b] = g.cover();
    assert(int(a.size() + b.size()) == ans);
    V < int > left(g.n + 1), right(g.m + 1);
    for (int u : a) {
        assert(u >= 1 && u <= g.n && ! left[u]);
        left[u] = 1;
    }
    for (int v : b) {
        assert(v >= 1 && v <= g.m && ! right[v]);
        right[v] = 1;
    }
    int cnt = 0;
    for (int u = 1; u <= g.n; ++ u) {
        int v = g.ml[u];
        assert(v >= 0 && v <= g.m);
        if (v) {
            assert(g.mr[v] == u);
            assert(find(g.g[u].begin(), g.g[u].end(), v) != g.g[u].end());
            cnt ++;
        }
        for (int x : g.g[u]) assert(left[u] || right[x]);
    }
    for (int v = 1; v <= g.m; ++ v) {
        int u = g.mr[v];
        assert(u >= 0 && u <= g.n);
        if (u) assert(g.ml[u] == v);
    }
    assert(cnt == ans);
}

void check_hk_cover() {
    HK g;
    assert(g.solve() == 0);
    check_cover_plan(g, 0);
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 8), m = rnd(0, 8), density = rnd(0, 100);
        g.init(n, m);
        for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= m; ++ v) if (rnd(0, 99) < density) {
                g.add_edge(u, v);
                if (rnd(0, 1)) g.add_edge(u, v);
            }
        int ans = brute_bipartite(g);
        assert(g.solve() == ans);
        check_cover_plan(g, ans);
        assert(g.solve() == ans);
        check_cover_plan(g, ans);
    }
}

int main() {
    check_dmst();
    check_blossom();
    check_shortest_paths();
    check_mincut();
    check_bfs01();
    check_hk_cover();
    cout << "extra_graph_random OK: 7 algorithms x 2000 rounds + boundaries\n";
}
