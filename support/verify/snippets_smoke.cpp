#include <bits/stdc++.h>

#include "../../template/others/常用定义.cpp"
#include "../../template/ds/并查集.cpp"
#include "../../template/ds/树状数组.cpp"
#include "../../template/ds/李超线段树.cpp"
#include "../../template/ds/可撤销并查集.cpp"
#include "../../template/ds/线段树与树上二分.cpp"
#include "../../template/ds/吉司机线段树.cpp"
#include "../../template/ds/可持久化数组.cpp"
#include "../../template/graph/堆优化最短路.cpp"
#include "../../template/graph/树上启发式合并.cpp"
#include "../../template/graph/最小生成树.cpp"
#include "../../template/graph/点双连通分量.cpp"
#include "../../template/graph/边双连通分量.cpp"
#include "../../template/graph/欧拉序最近公共祖先.cpp"
#include "../../template/graph/换根动态规划与距离和.cpp"
#include "../../template/graph/拓扑排序.cpp"
#include "../../template/graph/树的直径.cpp"
#include "../../template/graph/二分图匹配.cpp"
#include "../../template/graph/桥.cpp"
#include "../../template/graph/割点.cpp"
#include "../../template/graph/差分约束.cpp"
#include "../../template/graph/最大流.cpp"
#include "../../template/graph/有向图欧拉路.cpp"
#include "../../template/graph/无向图欧拉路.cpp"
#include "../../template/graph/倍增最近公共祖先.cpp"
#include "../../template/graph/势能最短路费用流.cpp"
#include "../../template/graph/容量缩放费用流.cpp"
#include "../../template/graph/强连通分量.cpp"
#include "../../template/math/自适应辛普森积分.cpp"
#include "../../template/math/最短线性递推.cpp"
#include "../../template/math/拉格朗日插值.cpp"
#include "../../template/math/矩阵快速幂.cpp"
#include "../../template/math/组合数.cpp"
#include "../../template/math/快速沃尔什变换.cpp"
#include "../../template/math/线性筛积性函数.cpp"
#include "../../template/math/快速数论变换与多项式乘法.cpp"
#include "../../template/math/线性筛素数.cpp"
#include "../../template/math/整除分块.cpp"
#include "../../template/math/异或线性基.cpp"
#include "../../template/others/笛卡尔树.cpp"
#include "../../template/others/莫队.cpp"
#include "../../template/others/滑动窗口最值.cpp"
#include "../../template/others/稀疏表.cpp"
#include "../../template/others/三分搜索.cpp"
#include "../../template/others/离散化.cpp"
#include "../../template/others/树哈希.cpp"
#include "../../template/string/字符串匹配.cpp"
#include "../../template/string/最长回文子串.cpp"
#include "../../template/string/后缀数组.cpp"
#include "../../template/string/多模式匹配自动机.cpp"
#include "../../template/string/后缀自动机.cpp"
#include "../../template/string/字典树.cpp"
#include "../../template/ds/带权与异或并查集.cpp"
#include "../../template/ds/主席树.cpp"
#include "../../template/ds/无旋平衡树.cpp"
#include "../../template/ds/二进制字典树.cpp"
#include "../../template/graph/树链剖分.cpp"
#include "../../template/graph/二元可满足性.cpp"
#include "../../template/graph/虚树.cpp"
#include "../../template/graph/点分治.cpp"
#include "../../template/graph/点分树.cpp"
#include "../../template/graph/最小斯坦纳树.cpp"
#include "../../template/graph/上下界网络流.cpp"
#include "../../template/graph/二分图最大权匹配.cpp"
#include "../../template/graph/分层增广二分图匹配.cpp"
#include "../../template/math/扩展欧几里得与中国剩余定理.cpp"
#include "../../template/math/离散对数.cpp"
#include "../../template/math/卢卡斯定理.cpp"
#include "../../template/math/素性测试与整数分解.cpp"
#include "../../template/math/原根.cpp"
#include "../../template/math/快速幂与逆元.cpp"
#include "../../template/math/矩阵求逆.cpp"
#include "../../template/ds/左偏树与可并堆.cpp"
#include "../../template/others/单调栈与左右最近更小与更大.cpp"
#include "../../template/others/单调凸包与斜率优化.cpp"
#include "../../template/math/高斯消元.cpp"
#include "../../template/math/多项式基本运算.cpp"
#include "../../template/math/多项式多点求值与快速插值.cpp"
#include "../../template/math/子集卷积.cpp"
#include "../../template/graph/重构树.cpp"
#include "../../template/graph/长链剖分与祖先查询.cpp"
#include "../../template/graph/圆方树.cpp"
#include "../../template/graph/边双缩点树.cpp"
#include "../../template/graph/最小割树.cpp"
#include "../../template/math/快速傅里叶变换与任意模数卷积.cpp"
#include "../../template/string/扩展字符串匹配.cpp"
#include "../../template/string/字符串哈希.cpp"
#include "../../template/string/回文自动机.cpp"
#include "../../template/string/最小表示法.cpp"
#include "../../template/others/分治三维偏序.cpp"
#include "../../template/others/整体二分与动态区间第几小.cpp"
#include "../../template/ds/仿射线段树.cpp"
#include "../../template/ds/二维树状数组.cpp"
#include "../../template/ds/动态开点线段树合并与分裂.cpp"
#include "../../template/ds/动态树.cpp"
#include "../../template/graph/零一最短路.cpp"
#include "../../template/graph/全源最短路.cpp"
#include "../../template/graph/重赋权全源最短路.cpp"
#include "../../template/graph/全局最小割.cpp"
#include "../../template/graph/最小树形图.cpp"
#include "../../template/graph/一般图匹配.cpp"
#include "../../template/graph/离线动态连通性.cpp"
#include "../../template/math/类欧几里得.cpp"
#include "../../template/math/二次剩余.cpp"
#include "../../template/math/杜教筛.cpp"
#include "../../template/math/扩展卢卡斯定理.cpp"
#include "../../template/math/行列式与矩阵树定理.cpp"
#include "../../template/others/带修改莫队.cpp"
#include "../../template/others/树上莫队.cpp"
#include "../../template/others/决策单调性.cpp"
#include "../../template/others/单调队列多重背包.cpp"
#include "../../template/string/位集优化最长公共子序列.cpp"
#include "../../template/ds/可持久化二进制字典树.cpp"
#include "../../template/ds/二维最近邻.cpp"
#include "../../template/graph/第几短游走.cpp"
#include "../../template/graph/上下界最小费用流.cpp"
#include "../../template/graph/支配树.cpp"
#include "../../template/graph/一般图最大权匹配.cpp"
#include "../../template/graph/最大团与最大独立集.cpp"
#include "../../template/math/积性函数求和.cpp"
#include "../../template/math/单纯形与线性规划.cpp"
#include "../../template/string/林登分解.cpp"
#include "../../template/string/极大周期子串.cpp"
#include "../../template/others/舞蹈链精确覆盖.cpp"
#include "../../template/geometry/点与向量.cpp"
#include "../../template/geometry/直线与线段.cpp"
#include "../../template/geometry/多边形.cpp"
#include "../../template/geometry/凸包与直径与闵可夫斯基和.cpp"
#include "../../template/geometry/凸多边形包含查询.cpp"
#include "../../template/geometry/平面最近点对.cpp"
#include "../../template/geometry/圆.cpp"
#include "../../template/geometry/半平面交.cpp"
#include "../../template/geometry/最小圆覆盖.cpp"
#include "../../template/geometry/矩形面积并与周长并.cpp"
#include "../../template/geometry/圆面积并与周长并.cpp"
#include "../../template/geometry/三维凸包.cpp"
#include "../../template/geometry/二维凸包最大点积查询.cpp"
#include "../../template/geometry/二维动态凸包.cpp"
#include "../../template/ds/区间集合与最小未出现数.cpp"
#include "../../template/graph/函数图分解与跳跃.cpp"
#include "../../template/math/等距拉格朗日插值.cpp"
#include "../../template/math/约数与倍数变换与卷积.cpp"
#include "../../template/math/两类斯特林数.cpp"
#include "../../template/math/线性递推快速求项.cpp"
#include "../../template/others/最长上升子序列与方案.cpp"
#include "../../template/others/博弈状态函数.cpp"
#include "../../template/string/二维哈希.cpp"
#include "../../template/geometry/最小面积包围矩形.cpp"
#include "../../template/geometry/嵌套凸多边形切线.cpp"

V < int > dsu_value, dsu_count, dsu_answer;
int dsu_distinct = 0;
void DSUOnTree::add(int u) {
    if (dsu_count[dsu_value[u]]++ == 0) ++dsu_distinct;
}
void DSUOnTree::del(int u) {
    if (--dsu_count[dsu_value[u]] == 0) --dsu_distinct;
}
void DSUOnTree::query(int u) { dsu_answer[u] = dsu_distinct; }

int main() {
    {
        NestedHull hull;
        hull.build({{0, 0}, {20, 0}, {0, 20}}, {{1, 1}, {3, 1}, {1, 3}});
        assert(hull.r == V < int > ({1, 2, 3, 4, 5, 6}));
        assert(hull.l == V < int > ({2, 3, 4, 5, 6, 7}));
    }
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
        AffineSeg seg; seg.init({0, 1, 2}, 7);
        seg.modify(1, 2, 2, 1); assert(seg.query(1, 2) == 1);
        V < int > q; assert(FPS::sqrt({1, 2, 1}, 3, q) && q[1] == 1);
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
        std::vector<int> key;
        auto id = discretize(a, key);
        assert((id == std::vector<int>{2, 1, 2}));
        assert((key == std::vector<int>{5, 10}));
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
        dsu_value = {0, 1, 2, 2, 3}; dsu_count.assign(4, 0); dsu_answer.assign(5, 0);
        dsu_distinct = 0;
        dsu.build(1);
        assert((dsu_answer == std::vector<int>{0, 3, 1, 2, 1}));
        assert(dsu_distinct == 0);
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
        V < int > order; assert(topo(graph, order));
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
        V < V < ll > > inverse; assert(matrix_inv({{0, 1}, {1, 0}}, 7, inverse));
        assert(primitive_root(17) == 3);
        auto [left, right] = monotonic_bounds({0, 3, 1, 1, 4});
        assert(left[4] == 3 && right[1] == 2);
        CHT hull; hull.add(2, 0); hull.add(1, 3); assert(hull.query(4) == 7);
        LeftistHeap heap;
        int root = heap.merge(heap.push(3), heap.push(1));
        assert(heap.top(root) == 1); root = heap.pop(root); assert(heap.top(root) == 3);
    }
    {
        assert((subset_conv({1, 2}, {3, 4}, 7) == V < int > {3, 3}));
        Multipoint poly; poly.build({0, 1, 2});
        assert((poly.eval({1, 2, 3}) == V < int > {1, 6, 17}));
        assert((poly.interp({1, 6, 17}) == V < int > {1, 2, 3}));
        KruskalTree tree(3); tree.add_edge(1, 2, 4); tree.add_edge(2, 3, 7);
        ll value; tree.build(); assert(tree.query(1, 3, value) && value == 7);
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
        PXorTrie trie;
        int a = trie.insert(0, 3), b = trie.insert(a, 5);
        unsigned long long value; assert(trie.max_xor(a, b, 2, value) && value == 7);
        KDTree kd; kd.build({{0, 0}, {2, 1}});
        pair < ll, int > nearest;
        assert(kd.nearest(1, 1, nearest) && nearest.second == 1 && kd.query(0, 0, 2, 1) == 2);
        KWalk walk(2); walk.add_edge(1, 2, 3); walk.add_edge(2, 2, 1);
        assert((walk.query(1, 2, 3) == V < ll > {3, 4, 5}));
        LRCostFlow flow(2); flow.add_edge(1, 2, 1, 3, -2);
        flow.add_edge(2, 1, 0, 3, 1);
        assert(flow.solve() == make_pair(true, -3ll));
        Dominator dom(3); dom.add_edge(1, 2); dom.add_edge(2, 3); dom.build(1);
        assert(dom.idom[3] == 2);
        WBlossom match(3);
        match.add_edge(1, 2, 5); match.add_edge(2, 3, 7); match.add_edge(1, 3, 6);
        assert(match.solve() == 7 && match.match[2] == 3);
        MaxClique clique(3); clique.add_edge(1, 2);
        assert(clique.clique().size() == 2);
        assert(clique.indep().size() == 2);
        Min25 sieve; sieve.init(100, 1000);
        assert(sieve.pi() == 25);
        assert(sieve.sum({1, 0, 0}, [](ll, int) { return 1; }) == 100);
        Simplex lp; auto optimum = lp.solve({{1, 1}}, {3}, {1, 2});
        assert(optimum.status == 1 && abs(optimum.value - 6) < 1e-9L);
        assert((duval("abab") == V < int > {0, 2, 4}));
        assert((runs("abab") == V < array < int, 3 > > {{2, 0, 4}}));
        DLX dlx(2); dlx.add_row(7, {1, 2});
        V < int > rows; assert((dlx.solve(rows) && rows == V < int > {7}));
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
        assert((gcd_conv({0, 1, 2}, {0, 3, 4}, 97) == V < ll > {0, 13, 8}));
        assert((lcm_conv({0, 1, 2}, {0, 3, 4}, 97) == V < ll > {0, 3, 18}));
        FuncGraph graph; graph.build({0, 2, 3, 2});
        assert(graph.jump(1, 100) == 3 && graph.dis(1, 3) == 2);
        assert((stirling1(3, 97)[3] == V < ll > {0, 2, 3, 1}));
        assert((stirling2_row(3) == V < ll > {0, 1, 3, 1}));
        Hash2D hash, small; hash.build({"ab", "cd"}); small.build({"b"});
        assert(hash.query(0, 1, 0, 1) == small.query(0, 0, 0, 0));
        auto rect = min_rectangle({{0, 0}, {3, 0}, {3, 2}, {0, 2}});
        assert(abs(rect.area - 6) < 1e-9L && rect.corners.size() == 4);
        IntervalSet seg; seg.insert(1, 4); seg.erase(2, 3);
        assert(seg.contains(1) && ! seg.contains(2) && seg.mex(1) == 2);
        V < int > sg;
        assert((grundy({{}, {2, 3}, {3}, {}}, sg) && sg == V < int > {0, 2, 1, 0}));
        assert(linear_kth({0, 1}, {1, 1}, 10) == 55);
    }
    return 0;
}
