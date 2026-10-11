#include <cassert>
#include "../../template/graph/KShortestWalk.cpp"

V < ll > brute(const KShortestWalk &kw, int s, int t, int k) {
    V < ll > ans;
    V < int > cnt(kw.n + 1);
    priority_queue < pair < ll, int >, V < pair < ll, int > >,
                     greater < pair < ll, int > > > q;
    q.push({0, s});
    while (! q.empty() && int(ans.size()) < k) {
        auto [d, u] = q.top(); q.pop();
        if (cnt[u] == k) continue;
        cnt[u] ++;
        if (u == t) ans.push_back(d);
        for (int id : kw.g[u]) {
            auto e = kw.edges[id];
            if (cnt[e.v] < k) q.push({d + e.w, e.v});
        }
    }
    return ans;
}

int main() {
    mt19937 rng(20261012);
    for (int test = 0; test < 1200; ++ test) {
        int n = int(rng() % 7) + 1, m = rng() % 30;
        KShortestWalk kw(n);
        for (int i = 0; i < m; ++ i)
            kw.add_edge(int(rng() % n) + 1, int(rng() % n) + 1, rng() % 6);
        for (int s = 1; s <= n; ++ s) for (int t = 1; t <= n; ++ t) {
            int k = int(rng() % 15) + 1;
            auto want = brute(kw, s, t, k);
            assert(kw.query(s, t, k) == want);
            assert(kw.query(s, t, k) == want);
        }
        assert(kw.query(1, n, 0).empty());
        kw.init(2); kw.add_edge(1, 2, 3); kw.add_edge(2, 1, 5);
        assert(kw.query(1, 2, 4) == V < ll > ({3, 11, 19, 27}));
    }
    KShortestWalk zero(1); zero.add_edge(1, 1, 0);
    assert(zero.query(1, 1, 1000) == V < ll > (1000, 0));
    KShortestWalk dag(10000);
    for (int u = 1; u < dag.n; ++ u) {
        dag.add_edge(u, u + 1, 1); dag.add_edge(u, u + 1, 1);
    }
    assert(dag.query(1, dag.n, 100) == V < ll > (100, dag.n - 1));
    KShortestWalk large(2); large.add_edge(1, 2, KShortestWalk::inf - 1);
    assert(large.query(1, 2, 2) == V < ll > ({KShortestWalk::inf - 1}));
    cout << "k shortest walk random tests passed\n";
}
