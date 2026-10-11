#include <cassert>
#include "../../template/graph/SCC.cpp"
#include "../../template/graph/CutVertex.cpp"
#include "../../template/graph/Bridge.cpp"
#include "../../template/graph/DSUOnTree.cpp"
#include "../../template/graph/LCA.cpp"
#include "../../template/graph/EulerLCA.cpp"
#include "../../template/graph/Reroot.cpp"
#include "../../template/graph/TreeDiameter.cpp"
#include "../../template/graph/EBCC.cpp"
#include "../../template/graph/DirectedEulerTrail.cpp"
#include "../../template/graph/UndirectedEulerTrail.cpp"
#include "../../template/others/TreeHash.cpp"
#include "../../template/others/MoTree.cpp"
#include "../../template/ds/LCT.cpp"
#include "../../template/graph/VBCC.cpp"
#include "../../template/graph/BlockCutTree.cpp"
#include "../../template/graph/BridgeTree.cpp"
#include "../../template/graph/KruskalTree.cpp"
#include "../../template/graph/KthAncestor.cpp"
#include "../../template/graph/FunctionalGraph.cpp"
#include "../../template/others/SG.cpp"

// Linux 下先提高栈限制，再验证 200000 点长链。
int main() {
    const int n = 200000;
    {
        V < int > to(n + 1);
        V < V < int > > g(n + 1);
        for (int i = 1; i < n; ++ i) to[i] = i + 1, g[i].push_back(i + 1);
        to[n] = n;
        FunctionalGraph graph; graph.build(to);
        assert(graph.dep[1] == n - 1 && graph.jump(1, ULLONG_MAX) == n);
        assert(graph.distance(1, n) == n - 1 && graph.distance(n, 1) == -1);
        auto sg = grundy(g); assert(sg && (*sg)[1] == (n - 1) % 2);
        g[n].push_back(1); assert(! grundy(g));
    }
    {
        KthAncestor tree(500000);
        for (int i = 1; i < tree.n; ++ i) tree.add_edge(i, i + 1);
        for (int root : {1, tree.n}) {
            tree.build(root);
            int u = root == 1 ? tree.n : 1;
            assert(tree.query(u, tree.n - 1) == root);
            assert(tree.query(u, tree.n) == 0);
        }
    }
    {
        KruskalTree tree(n);
        for (int i = 1; i < n; ++ i) tree.add_edge(i, i + 1, i);
        tree.build(); assert(tree.query(1, n) == n - 1);
        tree.build(true); assert(tree.query(1, n) == 1);
    }
    {
        VBCC graph(n);
        for (int i = 1; i < n; ++ i) graph.add_edge(i, i + 1);
        graph.build(); V < V < int > > blocks;
        for (const auto &part : graph.bcc) blocks.push_back(part.v);
        auto tree = block_cut_tree(n, blocks);
        assert(int(tree.size()) == 2 * n);
        int edges = 0;
        for (const auto &adj : tree) edges += int(adj.size());
        assert(edges == 4 * (n - 1));
    }
    {
        EBCC graph(n);
        for (int i = 1; i < n; ++ i) graph.add_edge(i, i + 1);
        graph.build(); auto tree = bridge_tree(graph.cnt, graph.bel, graph.edges);
        assert(int(tree.size()) == n + 1);
        int edges = 0;
        for (const auto &adj : tree) edges += int(adj.size());
        assert(edges == 2 * (n - 1));
    }
    {
        MoTree mo(n);
        for (int i = 1; i < n; ++ i) mo.add_edge(i, i + 1);
        for (int root : {1, n}) {
            mo.build(root);
            assert(mo.lca(1, n) == root);
            mo.query(1, n);
            int cnt = 0, queried = 0;
            mo.solve([&](int) { cnt ++; }, [&](int) { cnt --; }, [&](int id) {
                assert(id == 0 && cnt == n); queried ++;
            });
            assert(queried == 1);
        }
    }
    {
        LCT tree; tree.init(V < ll > (n + 1, 1));
        for (int i = 1; i < n; ++ i) assert(tree.link(i, i + 1));
        assert(tree.query(1, n) == n);
        tree.modify(n / 2, -1);
        assert(tree.query(n, 1) == n - 2);
        assert(tree.cut(n / 2, n / 2 + 1));
        assert(! tree.same(1, n));
        assert(tree.link(1, n));
        assert(tree.query(n / 2, n / 2 + 1) == n - 2);
    }
    {
        LCA lca(n);
        for (int i = 1; i < n; ++ i) lca.add_edge(i, i + 1);
        for (int root : {1, n, n / 2}) {
            lca.build(root);
            assert(lca.lca(1, n) == root);
            assert(lca.dep[1] == root - 1 && lca.dep[n] == n - root);
            assert(lca.fa[0][root] == 0);
            for (int j = 1; j < lca.lg; ++ j) assert(lca.fa[j][root] == 0);
        }
    }
    {
        EulerLCA lca(n);
        for (int i = 1; i < n; ++ i) lca.add_edge(i, i + 1);
        lca.build();
        assert(lca.lca(1, n) == 1);
        lca.build(n);
        assert(lca.lca(1, n) == n);
    }
    {
        Graph g(n + 1);
        for (int i = 1; i < n; ++ i) {
            g[i].push_back({i + 1, 1});
            g[i + 1].push_back({i, 1});
        }
        auto sum = reroot(g);
        assert(sum == reroot(g));
        for (int i = 1; i <= n; ++ i)
            assert(sum[i] == 1ll * i * (i - 1) / 2 + 1ll * (n - i) * (n - i + 1) / 2);
        auto [u, v, len] = tree_diameter(g);
        assert(len == n - 1 && ((u == 1 && v == n) || (u == n && v == 1)));
    }
    {
        EBCC ebcc(n);
        for (int i = 1; i < n; ++ i) ebcc.add_edge(i, i + 1);
        ebcc.add_edge(n, 1);
        for (int repeat = 0; repeat < 2; ++ repeat) {
            ebcc.build();
            assert(ebcc.cnt == 1 && ebcc.bcc.size() == 1);
            assert(int(ebcc.bcc[0].v.size()) == n && int(ebcc.bcc[0].e.size()) == n);
            for (int u = 1; u <= n; ++ u) assert(ebcc.bel[u] == 1);
            for (char op : ebcc.is_bridge) assert(! op);
        }
    }
    {
        V < pair < int, int > > edges;
        for (int i = 1; i < n; ++ i) edges.push_back({i, i + 1});
        auto d = euler_d(n, edges), u = euler_u(n, edges);
        assert(int(d.size()) == n && int(u.size()) == n);
        for (int i = 0; i < n; ++ i) assert(d[i] == i + 1 && u[i] == n - i);
        edges.assign(n, {1, 1});
        d = euler_d(1, edges); u = euler_u(1, edges);
        assert(int(d.size()) == n + 1 && int(u.size()) == n + 1);
        for (int x : d) assert(x == 1);
        for (int x : u) assert(x == 1);
    }
    {
        SCC scc(n);
        for (int i = 1; i < n; ++ i) scc.add_edge(i, i + 1);
        assert(scc.build() == n);
        assert(scc.build() == n);
    }
    {
        CutVertex cut(n);
        for (int i = 1; i < n; ++ i) cut.add_edge(i, i + 1);
        auto ans = cut.find();
        assert(int(ans.size()) == n - 2 && ans.front() == 2 && ans.back() == n - 1);
    }
    {
        Bridge bridge(n);
        for (int i = 1; i < n; ++ i) bridge.add_edge(i, i, i + 1);
        assert(int(bridge.find().size()) == n - 1);
    }
    {
        DSUOnTree dsu(n);
        for (int i = 1; i < n; ++ i) dsu.add_edge(i, i + 1);
        int cnt = 0, queried = 0;
        dsu.build(1, [&](int) { cnt ++; }, [&](int) { cnt --; }, [&](int u) {
            assert(cnt == n - u + 1);
            queried ++;
        });
        assert(cnt == 0 && queried == n);
    }
    {
        V < V < int > > g(n + 1);
        for (int i = 1; i < n; ++ i) {
            g[i].push_back(i + 1);
            g[i + 1].push_back(i);
        }
        assert(tree_hash(g, 1) == tree_hash(g, n));
    }
}
