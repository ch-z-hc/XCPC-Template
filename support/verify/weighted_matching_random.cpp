#include <cassert>
#include "../../template/graph/WeightedMatching.cpp"

ll brute(const V < V < ll > > &g, int mask, V < ll > &dp) {
    if (! mask) return 0;
    if (dp[mask] != -1) return dp[mask];
    int u = __builtin_ctz(unsigned(mask)), rest = mask ^ (1 << u);
    ll ans = brute(g, rest, dp);
    for (int v = u + 1; v < int(g.size()) - 1; ++ v)
        if (rest >> v & 1)
            ans = max(ans, g[u + 1][v + 1]
                      + brute(g, rest ^ (1 << v), dp));
    return dp[mask] = ans;
}

void check(WeightedBlossom &blossom, const V < V < ll > > &g) {
    int n = blossom.n;
    V < ll > dp(1 << n, -1);
    ll expected = brute(g, (1 << n) - 1, dp);
    for (int rep = 0; rep < 2; ++ rep) {
        ll ans = blossom.solve(), sum = 0;
        assert(ans == expected);
        for (int u = 1; u <= n; ++ u) {
            int v = blossom.match[u];
            assert(v >= 0 && v <= n && v != u);
            if (! v) continue;
            assert(blossom.match[v] == u && g[u][v] > 0);
            if (u < v) sum += g[u][v];
        }
        assert(sum == ans);
    }
}

void test_random() {
    mt19937_64 rng(20261011);
    WeightedBlossom blossom;
    for (int test = 0; test < 20000; ++ test) {
        int n = int(rng() % 13);
        blossom.init(n);
        V < V < ll > > g(n + 1, V < ll > (n + 1));
        int m = n ? int(rng() % (2 * n * n + 1)) : 0;
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            ll w = ll(rng() % 51) - 20;
            if (test % 5 == 0) w = ll(rng() % 3) - 1;
            if (test % 11 == 0) w *= 1000000000000ll;
            if (test % 17 == 0 && w > 0)
                w = ll(rng() % (LLONG_MAX / (8ll * n))) + 1;
            blossom.add_edge(u, v, w);
            if (u != v) g[u][v] = g[v][u] = max(g[u][v], w);
        }
        check(blossom, g);
        if (n > 1) {
            blossom.add_edge(1, n, 50);
            g[1][n] = g[n][1] = max(g[1][n], 50ll);
            check(blossom, g);
        }
    }
}

void test_exhaustive() {
    WeightedBlossom blossom(5);
    for (int mask = 0; mask < 59049; ++ mask) {
        blossom.init(5);
        V < V < ll > > g(6, V < ll > (6));
        int x = mask;
        for (int u = 1; u <= 5; ++ u)
            for (int v = u + 1; v <= 5; ++ v) {
                ll w = x % 3; x /= 3;
                blossom.add_edge(u, v, w); g[u][v] = g[v][u] = w;
            }
        check(blossom, g);
    }
}

void test_regressions() {
    WeightedBlossom blossom(4);
    blossom.add_edge(1, 2, 100);
    blossom.add_edge(1, 3, 1); blossom.add_edge(2, 4, 1);
    assert(blossom.solve() == 100);
    assert(blossom.match[1] == 2 && ! blossom.match[3]);
    for (int n = 3; n <= 11; n += 2) {
        blossom.init(n);
        V < V < ll > > g(n + 1, V < ll > (n + 1));
        ll big = LLONG_MAX / (8ll * n);
        for (int u = 1; u <= n; ++ u) {
            int v = u % n + 1;
            blossom.add_edge(u, v, big); g[u][v] = g[v][u] = big;
            blossom.add_edge(u, u, LLONG_MAX);
            blossom.add_edge(u, v, LLONG_MIN);
        }
        check(blossom, g);
    }
    blossom.init(2);
    blossom.add_edge(1, 2, LLONG_MAX / 16);
    assert(blossom.solve() == LLONG_MAX / 16);
    blossom.init(0); assert(blossom.solve() == 0);
}

void test_large() {
    const int n = 500;
    WeightedBlossom blossom(n);
    for (int u = 1; u <= n; ++ u)
        for (int v = u + 1; v <= n; ++ v)
            blossom.add_edge(u, v, 1000000);
    assert(blossom.solve() == 250000000);
    assert(blossom.solve() == 250000000);
    blossom.init(n);
    for (int u = 1; u < n; ++ u) blossom.add_edge(u, u + 1, u);
    assert(blossom.solve() == 62500);
}

void test_library_checker() {
    // Library Checker 的 issue610 和 anti53256 历史回归。
    const int a[][2] = {
        {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11},
        {1, 3}, {7, 9}, {0, 13}, {6, 12}, {1, 2}, {3, 4},
        {0, 6}, {7, 8}, {9, 10}, {5, 13}, {11, 12}
    };
    WeightedBlossom blossom(14);
    V < V < ll > > g(15, V < ll > (15));
    for (auto &e : a) {
        int u = e[0] + 1, v = e[1] + 1;
        blossom.add_edge(u, v, 1); g[u][v] = g[v][u] = 1;
    }
    check(blossom, g);
    const int b[][3] = {
        {12, 10, 6}, {13, 0, 10}, {1, 0, 6}, {8, 12, 5},
        {5, 14, 3}, {2, 8, 1}, {2, 11, 5}, {7, 10, 1},
        {0, 7, 7}, {7, 12, 2}, {14, 12, 3}, {6, 12, 10},
        {12, 5, 5}, {8, 16, 5}, {7, 3, 5}, {5, 7, 10},
        {17, 12, 7}, {16, 11, 2}, {6, 11, 9}, {0, 14, 2},
        {9, 17, 8}, {12, 11, 2}, {3, 0, 9}, {13, 3, 7},
        {9, 2, 5}, {14, 17, 9}, {1, 15, 3}, {16, 12, 3},
        {2, 17, 7}, {17, 5, 2}, {8, 5, 1}, {5, 9, 7},
        {14, 13, 5}, {7, 4, 1}, {1, 6, 6}, {14, 2, 7},
        {7, 9, 5}, {12, 9, 10}, {13, 6, 3}, {13, 12, 2},
        {13, 2, 7}, {4, 13, 10}, {16, 10, 3}, {2, 0, 7},
        {9, 10, 6}, {7, 16, 8}, {4, 17, 2}, {7, 17, 1},
        {10, 1, 4}, {7, 2, 3}, {11, 7, 6}, {11, 0, 10},
        {2, 1, 7}, {1, 5, 3}, {4, 0, 1}, {2, 6, 7},
        {1, 12, 3}, {4, 11, 5}, {0, 10, 2}, {1, 13, 9},
        {5, 11, 8}, {14, 3, 7}, {15, 5, 3}, {4, 8, 5},
        {5, 13, 3}, {15, 8, 2}, {10, 14, 2}, {3, 15, 3},
        {11, 14, 2}, {10, 11, 1}, {10, 2, 8}, {6, 15, 8},
        {11, 8, 1}, {2, 16, 3}, {13, 16, 3}, {13, 8, 4},
        {9, 15, 4}, {14, 15, 10}, {4, 9, 6}, {2, 3, 7},
        {15, 12, 9}, {7, 1, 6}, {14, 16, 1}, {5, 3, 9},
        {17, 6, 1}, {12, 0, 10}, {1, 16, 3}, {17, 10, 10},
        {4, 14, 10}, {13, 17, 3}, {17, 0, 3}, {12, 4, 9},
        {3, 9, 6}, {13, 9, 3}, {7, 6, 3}, {7, 15, 10},
        {7, 14, 9}, {10, 13, 7}, {4, 5, 4}, {17, 15, 6},
        {15, 0, 5}, {16, 3, 6}, {14, 1, 1}, {16, 0, 7},
        {11, 15, 10}, {12, 3, 2}, {17, 8, 8}, {3, 10, 10},
        {17, 11, 10}, {0, 6, 2}, {11, 13, 6}, {3, 8, 6},
        {4, 10, 9}, {5, 2, 1}, {10, 5, 3}, {3, 17, 4},
        {1, 8, 1}, {15, 2, 1}, {4, 15, 7}, {15, 13, 1}
    };
    blossom.init(18); g.assign(19, V < ll > (19));
    for (auto &e : b) {
        int u = e[0] + 1, v = e[1] + 1; ll w = e[2];
        blossom.add_edge(u, v, w); g[u][v] = g[v][u] = w;
    }
    check(blossom, g);
}

int main() {
    test_regressions(); test_exhaustive(); test_random(); test_large();
    test_library_checker();
    return 0;
}
