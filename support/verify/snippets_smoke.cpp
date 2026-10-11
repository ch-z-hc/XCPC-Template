#include <cassert>
#include <string>
#include <vector>

#include "../../template/others/Common.cpp"
#include "../../template/ds/DSU.cpp"
#include "../../template/ds/Fenwick.cpp"
#include "../../template/ds/LiChaoTree.cpp"
#include "../../template/ds/RollbackDSU.cpp"
#include "../../template/ds/SegTree.cpp"
#include "../../template/ds/SegmentTreeBeats.cpp"
#include "../../template/ds/PersistentArray.cpp"
#include "../../template/graph/Dijkstra.cpp"
#include "../../template/graph/DSUOnTree.cpp"
#include "../../template/graph/Kruskal.cpp"
#include "../../template/graph/VBCC.cpp"
#include "../../template/graph/EBCC.cpp"
#include "../../template/graph/EulerLCA.cpp"
#include "../../template/graph/Reroot.cpp"
#include "../../template/graph/TopologicalSort.cpp"
#include "../../template/graph/TreeDiameter.cpp"
#include "../../template/graph/BipartiteMatch.cpp"
#include "../../template/graph/Bridge.cpp"
#include "../../template/graph/CutVertex.cpp"
#include "../../template/graph/DiffConstraints.cpp"
#include "../../template/graph/Dinic.cpp"
#include "../../template/graph/DirectedEulerTrail.cpp"
#include "../../template/graph/UndirectedEulerTrail.cpp"
#include "../../template/graph/LCA.cpp"
#include "../../template/graph/MinCostFlow.cpp"
#include "../../template/graph/ScalingMinCostFlow.cpp"
#include "../../template/graph/SCC.cpp"
#include "../../template/math/AdaptiveSimpson.cpp"
#include "../../template/math/BerlekampMassey.cpp"
#include "../../template/math/Lagrange.cpp"
#include "../../template/math/Matrix.cpp"
#include "../../template/math/Combination.cpp"
#include "../../template/math/FWT.cpp"
#include "../../template/math/LinearSieve.cpp"
#include "../../template/math/NTT.cpp"
#include "../../template/math/PrimeSieve.cpp"
#include "../../template/math/QuotientBlocks.cpp"
#include "../../template/math/XorBasis.cpp"
#include "../../template/others/CartesianTree.cpp"
#include "../../template/others/Mo.cpp"
#include "../../template/others/SlidingWindow.cpp"
#include "../../template/others/SparseTable.cpp"
#include "../../template/others/TernarySearch.cpp"
#include "../../template/others/Discretize.cpp"
#include "../../template/others/TreeHash.cpp"
#include "../../template/string/KMP.cpp"
#include "../../template/string/Manacher.cpp"
#include "../../template/string/SuffixArray.cpp"
#include "../../template/string/ACAutomaton.cpp"
#include "../../template/string/SAM.cpp"
#include "../../template/string/Trie.cpp"
#include "../../template/ds/WeightedDSU.cpp"
#include "../../template/ds/PersistentSegTree.cpp"
#include "../../template/ds/FHQTreap.cpp"
#include "../../template/ds/XorTrie.cpp"
#include "../../template/graph/HLD.cpp"
#include "../../template/graph/TwoSAT.cpp"
#include "../../template/graph/VirtualTree.cpp"
#include "../../template/graph/Centroid.cpp"
#include "../../template/graph/CentroidTree.cpp"
#include "../../template/graph/SteinerTree.cpp"
#include "../../template/graph/LowerBoundFlow.cpp"
#include "../../template/graph/KM.cpp"
#include "../../template/graph/HopcroftKarp.cpp"
#include "../../template/math/CRT.cpp"
#include "../../template/math/BSGS.cpp"
#include "../../template/math/Lucas.cpp"
#include "../../template/math/PollardRho.cpp"
#include "../../template/math/PrimitiveRoot.cpp"
#include "../../template/math/ModArithmetic.cpp"
#include "../../template/math/MatrixInverse.cpp"
#include "../../template/ds/LeftistHeap.cpp"
#include "../../template/others/MonotonicStack.cpp"
#include "../../template/others/ConvexHullTrick.cpp"
#include "../../template/math/Gauss.cpp"
#include "../../template/math/FPS.cpp"
#include "../../template/math/Multipoint.cpp"
#include "../../template/math/SubsetConvolution.cpp"
#include "../../template/graph/KruskalTree.cpp"
#include "../../template/graph/KthAncestor.cpp"
#include "../../template/graph/BlockCutTree.cpp"
#include "../../template/graph/BridgeTree.cpp"
#include "../../template/graph/GomoryHu.cpp"
#include "../../template/math/FFT.cpp"
#include "../../template/string/Z.cpp"
#include "../../template/string/Hash.cpp"
#include "../../template/string/PAM.cpp"
#include "../../template/string/MinRotation.cpp"
#include "../../template/others/CDQ.cpp"
#include "../../template/others/OverallBinary.cpp"
#include "../../template/ds/AffineSegTree.cpp"
#include "../../template/ds/Fenwick2D.cpp"
#include "../../template/ds/MergeSegTree.cpp"
#include "../../template/ds/LCT.cpp"
#include "../../template/graph/ZeroOneBFS.cpp"
#include "../../template/graph/Floyd.cpp"
#include "../../template/graph/Johnson.cpp"
#include "../../template/graph/GlobalMinCut.cpp"
#include "../../template/graph/DirectedMST.cpp"
#include "../../template/graph/GeneralMatching.cpp"
#include "../../template/graph/DynamicConnectivity.cpp"
#include "../../template/math/FloorSum.cpp"
#include "../../template/math/ModSqrt.cpp"
#include "../../template/math/DuJiao.cpp"
#include "../../template/math/ExLucas.cpp"
#include "../../template/math/Determinant.cpp"
#include "../../template/others/MoModify.cpp"
#include "../../template/others/MoTree.cpp"
#include "../../template/others/DPOpt.cpp"
#include "../../template/string/BitsetLCS.cpp"
#include "../../template/ds/PersistentXorTrie.cpp"
#include "../../template/ds/KDTree.cpp"
#include "../../template/graph/KShortestWalk.cpp"
#include "../../template/graph/LowerBoundMinCostFlow.cpp"
#include "../../template/graph/Dominator.cpp"
#include "../../template/graph/WeightedMatching.cpp"
#include "../../template/graph/MaxClique.cpp"
#include "../../template/math/Min25.cpp"
#include "../../template/math/Simplex.cpp"
#include "../../template/string/Lyndon.cpp"
#include "../../template/string/Runs.cpp"
#include "../../template/others/DLX.cpp"
#include "../../template/geometry/Point.cpp"
#include "../../template/geometry/Line.cpp"
#include "../../template/geometry/Polygon.cpp"
#include "../../template/geometry/ConvexHull.cpp"
#include "../../template/geometry/ConvexQuery.cpp"
#include "../../template/geometry/ClosestPair.cpp"
#include "../../template/geometry/Circle.cpp"
#include "../../template/geometry/HalfPlane.cpp"
#include "../../template/geometry/MinCircle.cpp"
#include "../../template/geometry/RectangleUnion.cpp"
#include "../../template/geometry/CircleUnion.cpp"
#include "../../template/geometry/ConvexHull3D.cpp"
#include "../../template/ds/IntervalSet.cpp"
#include "../../template/graph/FunctionalGraph.cpp"
#include "../../template/math/EquidistantLagrange.cpp"
#include "../../template/math/DivisorTransform.cpp"
#include "../../template/math/Stirling.cpp"
#include "../../template/math/BostanMori.cpp"
#include "../../template/others/LIS.cpp"
#include "../../template/others/SG.cpp"
#include "../../template/string/Hash2D.cpp"
#include "../../template/geometry/MinRectangle.cpp"

int main() {
    {
        Beats seg; seg.init({0, 1, 5, -2});
        seg.chmin(1, 3, 3); seg.chmax(2, 3, 0); seg.add(1, 2, 2);
        assert(seg.query(1, 3) == 8);
        assert(seg.query_min(1, 3) == 0 && seg.query_max(1, 3) == 5);
        PArray a; a.init({0, 7, 9});
        int p = a.modify(a.root, 1, -3);
        assert(a.query(a.root, 1) == 7 && a.query(p, 1) == -3);
        CentroidTree tree(3); tree.add_edge(1, 2); tree.add_edge(2, 3);
        tree.build(); tree.modify(1, 4); tree.modify(3, -1);
        assert(tree.query(2, 1) == 3 && tree.query(1, 0) == 4);
        tree.build(); assert(tree.query(2, 3) == 0);
        Steiner graph(4);
        graph.add_edge(1, 4, 1); graph.add_edge(2, 4, 2);
        graph.add_edge(3, 4, 3);
        assert(graph.solve({1, 2, 3}) == 6 && graph.solve({}) == 0);
    }
    {
        Fenwick bit(3); bit.add(2, 1); assert(bit.kth(1) == 2);
        AffineSegTree seg; seg.init({0, 1, 2}, 7);
        seg.modify(1, 2, 2, 1); assert(seg.query(1, 2) == 1);
        auto q = FPS::sqrt({1, 2, 1}, 3); assert(q && (*q)[1] == 1);
        assert(bitset_lcs("abc", "ac") == 2);
        ClosestPair cp; cp.build({P(0, 0), P(3, 4)}); assert(cp.d2 == 25);
        ConvexQuery hull; hull.build({P(0, 0), P(2, 0), P(0, 2)});
        assert(hull.query(P(0.5, 0.5)) == 2);
    }
    {
        std::vector<long long> a{1};
        ntt(a);
        assert(a[0] == 1);
    }
    {
        Fenwick fenwick(std::vector<long long>{0, 1, 2, 3});
        fenwick.add(2, 4);
        assert(fenwick.query(2, 3) == 9);
        DSU dsu(3);
        dsu.merge(1, 2);
        assert(dsu.same(1, 2) && !dsu.same(1, 3));
        assert(dsu.size(1) == 2 && dsu.cnt == 2);
    }
    {
        std::vector<int> a{10, 5, 10};
        auto id = discretize(a);
        assert((id == std::vector<int>{2, 1, 2}));
    }
    {
        SegTree st;
        st.init(std::vector<long long>{0, 1, 2, 3});
        st.modify(1, 2, 5);
        assert(st.query(1, 3) == 16);
    }
    {
        Graph g(4);
        g[1] = {{2, 5}, {3, 10}};
        g[2] = {{3, 2}};
        assert((dijkstra(g, 1) == std::vector<long long>{1LL << 62, 0, 5, 7}));
    }
    {
        std::string s = "banana";
        SA suffix_array(s);
        assert((suffix_array.sa == std::vector<int>{5, 3, 1, 0, 4, 2}));
        assert(suffix_array.lcp(1, 3) == 3);
    }
    {
        RollbackDSU dsu(3);
        int snap = dsu.snapshot();
        assert(dsu.merge(1, 2));
        assert(dsu.find(1) == dsu.find(2));
        dsu.rollback(snap);
        assert(dsu.find(1) != dsu.find(2));
    }
    {
        Graph g(5);
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
        EulerLCA lca(3);
        lca.add_edge(1, 2); lca.add_edge(2, 3); lca.build();
        assert(lca.lca(1, 3) == 1);
    }
    {
        auto mst = kruskal(3, {{1, 2, 1, 3}, {2, 3, 2, 4}, {1, 3, 3, 10}});
        assert(mst.ok && mst.sum == 7);
        std::vector<std::vector<int>> graph{{}, {2}, {3}, {}};
        assert(topo(graph).has_value());
        assert((kmp("ababa", "aba") == std::vector<int>{0, 2}));
        Manacher manacher("abacaba");
        assert(manacher.query(0, 6));
    }
    {
        VBCC vertex_components(4);
        vertex_components.add_edge(1, 2);
        vertex_components.add_edge(2, 3);
        vertex_components.add_edge(3, 1);
        vertex_components.add_edge(3, 4);
        vertex_components.build();
        assert(vertex_components.bcc.size() == 2);
        bool found_triangle = false, found_bridge = false;
        for (const auto &part : vertex_components.bcc) {
            if (part.v.size() == 3 && part.e.size() == 3)
                found_triangle = true;
            if (part.v.size() == 2 && part.e.size() == 1)
                found_bridge = true;
        }
        assert(found_triangle && found_bridge);

        EBCC edge_components(4);
        edge_components.add_edge(1, 2);
        edge_components.add_edge(2, 3);
        edge_components.add_edge(3, 1);
        int bridge = edge_components.add_edge(3, 4);
        edge_components.build();
        assert(edge_components.cnt == 2);
        assert(edge_components.bel[1] == edge_components.bel[2]);
        assert(edge_components.bel[3] != edge_components.bel[4]);
        assert(edge_components.is_bridge[bridge]);
        for (const auto &part : edge_components.bcc) {
            if (part.v.size() == 3)
                assert(part.e.size() == 3);
            else if (part.v.size() == 1)
                assert(part.e.empty());
            else
                assert(false);
        }
    }
    {
        Graph g(4);
        g[1].push_back({2, 1}); g[2].push_back({1, 1});
        g[2].push_back({3, 1}); g[3].push_back({2, 1});
        auto sum = reroot(g);
        assert((sum == std::vector<long long>{0, 3, 2, 3}));
        g[1][0].second = g[2][0].second = -2;
        g[2][1].second = g[3][0].second = 3;
        assert((reroot(g) == std::vector<long long>{0, -1, 1, 4}));
    }
    {
        std::vector<std::vector<int>> a(3), b(3);
        a[1].push_back(2); a[2].push_back(1);
        b[1].push_back(2); b[2].push_back(1);
        assert(tree_hash(a) == tree_hash(b));
    }
    {
        const long long mod = 1000000007;
        Matrix a{{1, 1}, {1, 0}};
        assert(matrix_pow(a, 5, mod)[0][1] == 5);
        std::vector<long long> x{0, 1}, y{0, 1};
        assert(lagrange(x, y, 2, mod) == 2);
        std::vector<long long> seq{0, 1, 1, 2, 3, 5, 8, 13};
        auto rec = bm(seq, mod);
        assert((rec == std::vector<long long>{1, 1}));
        assert(kth({0, 1}, rec, 10, mod) == 55);
    }
    {
        assert(pow_mod(-2, 3, 7) == 6 && linear_inverse(3, 7)[3] == 5);
        assert((batch_inverse({2, 3}, 7) == V < ll > {4, 5}));
        assert(matrix_inv({{0, 1}, {1, 0}}, 7).has_value());
        assert(primitive_root(17) == 3);
        auto [left, right] = monotonic_bounds({0, 3, 1, 1, 4});
        assert(left[4] == 3 && right[1] == 2);
        CHT hull; hull.add(2, 0); hull.add(1, 3); assert(hull.query(4) == 7);
        LeftistHeap heap;
        int root = heap.merge(heap.push(3), heap.push(1));
        assert(heap.top(root) == 1); root = heap.pop(root); assert(heap.top(root) == 3);
    }
    {
        assert((subset_convolution({1, 2}, {3, 4}, 7) == V < int > {3, 3}));
        Multipoint poly; poly.build({0, 1, 2});
        assert((poly.evaluate({1, 2, 3}) == V < int > {1, 6, 17}));
        assert((poly.interpolate({1, 6, 17}) == V < int > {1, 2, 3}));
        KruskalTree tree(3); tree.add_edge(1, 2, 4); tree.add_edge(2, 3, 7);
        tree.build(); assert(tree.query(1, 3) == 7);
        KthAncestor ancestor(3); ancestor.add_edge(1, 2); ancestor.add_edge(2, 3);
        ancestor.build(); assert(ancestor.query(3, 2) == 1);
        auto block = block_cut_tree(3, {{1, 2}, {2, 3}});
        assert(block.size() == 6 && block[2].size() == 2);
        auto bridge = bridge_tree(2, {0, 1, 1, 2}, {{1, 2}, {2, 3}});
        assert(bridge[1][0] == make_pair(2, 1));
        GomoryHu cut(3); cut.add_edge(1, 2, 4); cut.add_edge(2, 3, 7);
        cut.build(); assert(cut.query(1, 3) == 4);
    }
    {
        PersistentXorTrie trie;
        int a = trie.insert(0, 3), b = trie.insert(a, 5);
        assert(trie.max_xor(a, b, 2) == 7);
        KDTree kd; kd.build({{0, 0}, {2, 1}});
        assert(kd.nearest(1, 1)->second == 1 && kd.query(0, 0, 2, 1) == 2);
        KShortestWalk walk(2); walk.add_edge(1, 2, 3); walk.add_edge(2, 2, 1);
        assert((walk.query(1, 2, 3) == V < ll > {3, 4, 5}));
        LRCostFlow flow(2); flow.add_edge(1, 2, 1, 3, -2);
        flow.add_edge(2, 1, 0, 3, 1);
        assert(flow.solve() == make_pair(true, -3ll));
        Dominator dom(3); dom.add_edge(1, 2); dom.add_edge(2, 3); dom.build(1);
        assert(dom.idom[3] == 2);
        WeightedBlossom match(3);
        match.add_edge(1, 2, 5); match.add_edge(2, 3, 7); match.add_edge(1, 3, 6);
        assert(match.solve() == 7 && match.match[2] == 3);
        MaxClique clique(3); clique.add_edge(1, 2);
        assert(clique.max_clique().size() == 2);
        assert(clique.max_independent_set().size() == 2);
        Min25 sieve; sieve.init(100, 1000);
        assert(sieve.count_primes() == 25);
        assert(sieve.sum({1, 0, 0}, [](ll, int) { return 1; }) == 100);
        Simplex lp; auto optimum = lp.solve({{1, 1}}, {3}, {1, 2});
        assert(optimum.status == 1 && abs(optimum.value - 6) < 1e-9L);
        assert((lyndon_factorization("abab") == V < int > {0, 2, 4}));
        assert((enumerate_runs("abab") == V < array < int, 3 > > {{2, 0, 4}}));
        DLX dlx(2); dlx.add_row(7, {1, 2});
        assert((dlx.solve() == optional < V < int > > {{7}}));
        auto circle = circle_union({{{0, 0}, 1}, {{0, 0}, 1}});
        assert(abs(circle.first - acosl(-1)) < 1e-9L);
        Hull3D hull; hull.build({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
        assert(hull.dim == 3 && abs(hull.volume() - 1.L / 6) < 1e-9L);
    }
    {
        LagrangeEq poly; poly.build({1, 4, 9}, 1000000007);
        assert(poly.query(3) == 16);
        SegTree tree; tree.init({0, 0, 2, 1});
        assert(tree.first_ge(1, 3, 2) == 2 && tree.last_ge(1, 3, 1) == 3);
        assert(tree.kth(3) == 3);
        assert(lis(V < int > {0, 2, 2, 3}).size() == 2);
        assert(lis(V < int > {0, 2, 2, 3}, false).size() == 3);
        assert((gcd_convolution({0, 1, 2}, {0, 3, 4}, 97) == V < ll > {0, 13, 8}));
        assert((lcm_convolution({0, 1, 2}, {0, 3, 4}, 97) == V < ll > {0, 3, 18}));
        FunctionalGraph graph; graph.build({0, 2, 3, 2});
        assert(graph.jump(1, 100) == 3 && graph.distance(1, 3) == 2);
        assert((stirling_first(3, 97)[3] == V < ll > {0, 2, 3, 1}));
        assert((stirling_second_row(3) == V < ll > {0, 1, 3, 1}));
        Hash2D hash, small; hash.build({"ab", "cd"}); small.build({"b"});
        assert(hash.query(0, 1, 0, 1) == small.query(0, 0, 0, 0));
        auto rect = min_rectangle({{0, 0}, {3, 0}, {3, 2}, {0, 2}});
        assert(abs(rect.area - 6) < 1e-9L && rect.corners.size() == 4);
        IntervalSet seg; seg.insert(1, 4); seg.erase(2, 3);
        assert(seg.contains(1) && ! seg.contains(2) && seg.mex(1) == 2);
        assert((grundy({{}, {2, 3}, {3}, {}}).value() == V < int > {0, 2, 1, 0}));
        assert(linear_kth({0, 1}, {1, 1}, 10) == 55);
    }
    return 0;
}
