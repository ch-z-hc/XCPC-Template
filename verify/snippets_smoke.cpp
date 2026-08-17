#include <cassert>
#include <string>
#include <vector>

#include "../ds/rollback_dsu.cpp"
#include "../ds/segment_tree.cpp"
#include "../graph/DSUOnTree.cpp"
#include "../graph/olca.cpp"
#include "../graph/reroot.cpp"
#include "../graph/tree_diameter.cpp"
#include "../math/bm.cpp"
#include "../math/interpolation.cpp"
#include "../math/matrix.cpp"
#include "../others/discretization.cpp"
#include "../others/tree_hash.cpp"
#include "../string/SA.cpp"

int main() {
    {
        std::vector<int> a{10, 5, 10};
        auto id = discretize(a);
        assert((id == std::vector<int>{2, 1, 2}));
    }
    {
        SegmentTree st;
        st.build(std::vector<long long>{0, 1, 2, 3});
        st.add(1, 2, 5);
        assert(st.query(1, 3) == 16);
    }
    {
        std::string s = "banana";
        std::vector<int> sa, rk, height, lg;
        std::vector<std::vector<int>> sparse;
        build_sa(s, sa, rk, height, lg, sparse);
        assert((sa == std::vector<int>{5, 3, 1, 0, 4, 2}));
        assert(lcp_sa(1, 3, rk, lg, sparse, int(s.size())) == 3);
    }
    {
        RollbackDSU dsu(3);
        int snap = dsu.snapshot();
        assert(dsu.unite(1, 2));
        assert(dsu.find(1) == dsu.find(2));
        dsu.rollback(snap);
        assert(dsu.find(1) != dsu.find(2));
    }
    {
        WeightedTree g(5);
        g[1].push_back({2, 1}); g[2].push_back({1, 1});
        g[2].push_back({3, 2}); g[3].push_back({2, 2});
        g[3].push_back({4, 3}); g[4].push_back({3, 3});
        auto [u, v, d] = tree_diameter(g);
        assert(((u == 1 && v == 4) || (u == 4 && v == 1)) && d == 6);
    }
    {
        DSUOnTree dsu(4);
        dsu.add_edge(1, 2); dsu.add_edge(1, 3); dsu.add_edge(3, 4);
        std::vector<int> value{0, 1, 2, 2, 3}, count(4), answer(5);
        int distinct = 0;
        auto add = [&](int u) { if (count[value[u]]++ == 0) ++distinct; };
        auto remove = [&](int u) { if (--count[value[u]] == 0) --distinct; };
        auto query = [&](int u) { answer[u] = distinct; };
        dsu.build(1, add, remove, query);
        assert((answer == std::vector<int>{0, 3, 1, 2, 1}));
        assert(distinct == 0);
    }
    {
        O1LCA lca(3);
        lca.add_edge(1, 2); lca.add_edge(2, 3); lca.build();
        assert(lca.lca(1, 3) == 1);
    }
    {
        RerootTree g(4);
        g[1].push_back({2, 1}); g[2].push_back({1, 1});
        g[2].push_back({3, 1}); g[3].push_back({2, 1});
        auto sum = tree_distance_sums(g);
        assert((sum == std::vector<long long>{0, 3, 2, 3}));
    }
    {
        std::vector<std::vector<int>> a(3), b(3);
        a[1].push_back(2); a[2].push_back(1);
        b[1].push_back(2); b[2].push_back(1);
        assert(rooted_tree_hash(1, 0, a) == rooted_tree_hash(1, 0, b));
    }
    {
        const long long mod = 1000000007;
        Matrix a{{1, 1}, {1, 0}};
        assert(matrix_pow(a, 5, mod)[0][1] == 5);
        std::vector<long long> x{0, 1}, y{0, 1};
        assert(interpolation_eval(x, y, 2, mod) == 2);
        std::vector<long long> seq{0, 1, 1, 2, 3, 5, 8, 13};
        auto rec = berlekamp_massey(seq, mod);
        assert((rec == std::vector<long long>{1, 1}));
        assert(linear_recurrence_kth({0, 1}, rec, 10, mod) == 55);
    }
    return 0;
}
