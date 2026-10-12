#include <cassert>
#include "../../template/graph/最大流.cpp"
#include "../../template/graph/势能最短路费用流.cpp"
#include "../../template/graph/容量缩放费用流.cpp"

struct TestEdge { int u, v, cap, cost; };

pair < ll, ll > brute_flow(int n, const V < TestEdge > &edges) {
    V < int > bal(n + 1);
    pair < ll, ll > ans{-1, 0};
    auto dfs = [&](auto &&self, int i, ll cost) -> void {
        if (i == int(edges.size())) {
            for (int u = 2; u < n; ++ u) if (bal[u]) return;
            if (bal[1] < 0 || bal[n] != -bal[1]) return;
            if (bal[1] > ans.first || (bal[1] == ans.first && cost < ans.second))
                ans = {bal[1], cost};
            return;
        }
        auto [u, v, cap, w] = edges[i];
        for (int f = 0; f <= cap; ++ f) {
            bal[u] += f; bal[v] -= f;
            self(self, i + 1, cost + f * w);
            bal[u] -= f; bal[v] += f;
        }
    };
    dfs(dfs, 0, 0);
    return ans;
}

int main() {
    mt19937 rng(20261010);
    for (int test = 0; test < 600; ++ test) {
        int n = int(rng() % 4) + 2, m = rng() % 8;
        V < TestEdge > edges;
        Dinic dinic(n);
        MCMF mcf(n);
        ScalingMCMF scaling(n);
        // 非负边费图或 DAG 可用常规费用流；任意有向图用于测试负环版本。
        bool ordinary = test % 3 != 2;
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            int cap = rng() % 3, cost = int(rng() % 9) - 4;
            if (test % 3 == 0) cost = abs(cost);
            if (test % 3 == 1) {
                if (u == v) continue;
                if (u > v) swap(u, v);
            }
            edges.push_back({u, v, cap, cost});
            dinic.add_edge(u, v, cap);
            mcf.add_edge(u, v, cap, cost);
            scaling.add_edge(u, v, cap, cost);
        }
        auto ans = brute_flow(n, edges);
        assert(dinic.flow(1, n) == ans.first);
        assert(dinic.flow(1, n) == 0);
        assert(scaling.flow(1, n) == ans);
        assert(scaling.flow(1, n) == ans);
        if (ordinary) {
            assert(mcf.flow(1, n) == ans);
            assert(mcf.flow(1, n) == make_pair(0LL, 0LL));
        }
        for (int u = 1; u <= n; ++ u) {
            for (int i = 0; i < int(dinic.g[u].size()); ++ i) {
                auto e = dinic.g[u][i];
                assert(dinic.g[e.to][e.rev].to == u);
                assert(dinic.g[e.to][e.rev].rev == i);
            }
            for (int i = 0; i < int(mcf.g[u].size()); ++ i) {
                auto e = mcf.g[u][i];
                assert(mcf.g[e.to][e.rev].to == u);
                assert(mcf.g[e.to][e.rev].rev == i);
            }
        }
        dinic.init(2); dinic.add_edge(1, 2, 7);
        assert(dinic.flow(1, 2) == 7);
        mcf.init(2); mcf.add_edge(1, 2, 3, -2);
        assert(mcf.flow(1, 2) == make_pair(3LL, -6LL));
        scaling.init(2); scaling.add_edge(1, 2, 3, -2);
        assert(scaling.flow(1, 2) == make_pair(3LL, -6LL));
    }
    ScalingMCMF scaling(4);
    scaling.add_edge(1, 4, 2, 5);
    scaling.add_edge(2, 2, 3, -7);
    assert(scaling.flow(1, 4) == make_pair(2LL, -11LL));
    return 0;
}
