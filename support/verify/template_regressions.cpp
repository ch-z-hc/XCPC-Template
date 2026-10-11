#include <cassert>
#include <vector>

#include "../../template/ds/LiChaoTree.cpp"
#include "../../template/graph/Bridge.cpp"
#include "../../template/graph/CutVertex.cpp"
#include "../../template/graph/Dinic.cpp"
#include "../../template/graph/MinCostFlow.cpp"
#include "../../template/graph/BipartiteMatch.cpp"
#include "../../template/graph/ScalingMinCostFlow.cpp"
#include "../../template/math/BerlekampMassey.cpp"
#include "../../template/string/ACAutomaton.cpp"

int main() {
    {
        MCMF mcf(2);
        mcf.add_edge(1, 2, 1, 1LL << 62);
        assert(mcf.flow(1, 2) == std::make_pair(1LL, 1LL << 62));
        assert(mcf.flow(1, 2) == std::make_pair(0LL, 0LL));
        mcf.init(3);
        mcf.add_edge(1, 2, 1, -3000000000000000000LL);
        mcf.add_edge(2, 3, 1, 0);
        mcf.add_edge(1, 3, 1, 3000000000000000000LL);
        assert(mcf.flow(1, 3) == std::make_pair(2LL, 0LL));
        const ll lim = std::numeric_limits<ll>::max();
        mcf.init(2);
        mcf.add_edge(1, 2, lim, 1);
        assert(mcf.flow(1, 2) == std::make_pair(lim, lim));
        mcf.init(2);
        mcf.add_edge(1, 2, 1, lim);
        assert(mcf.flow(1, 2) == std::make_pair(1LL, lim));
        mcf.init(4);
        mcf.add_edge(1, 2, 1, -(lim - 10));
        mcf.add_edge(2, 4, 1, -(lim - 10));
        mcf.add_edge(1, 3, 1, lim - 10);
        mcf.add_edge(3, 4, 1, lim - 10);
        assert(mcf.flow(1, 4) == std::make_pair(2LL, 0LL));
    }
    {
        Kuhn match(40000, 40000);
        match.add_edge(1, 40000);
        assert(match.solve() == 1);
        assert(match.solve() == 1);
        match.init(2, 2);
        match.add_edge(1, 1); match.add_edge(1, 2); match.add_edge(2, 1);
        assert(match.solve() == 2);
    }
    {
        LiChaoTree tree(4);
        int id = tree.add_line(0, -1e20L);
        assert(tree.query_id(2) == id);
        assert(tree.query(2) == -1e20L);
    }
    {
        const long long mod = 1000000007;
        assert(kth({3}, {2}, 5, mod) == 96);
        assert(kth({0, 1}, {2, 3}, 2, mod) == 2);
    }
    {
        AC ac;
        ac.insert("a");
        ac.build();
        assert((ac.query("aa") == std::vector<long long>{2}));
        ac.build();
        ac.insert("aa");
        ac.build();
        assert((ac.query("aa") == std::vector<long long>{2, 1}));
    }
    {
        Bridge finder(2);
        finder.add_edge(1, 1, 2);
        assert((finder.find() == std::vector<int>{1}));
        finder.add_edge(2, 1, 2);
        assert(finder.find().empty());
    }
    {
        CutVertex finder(3);
        finder.add_edge(1, 2);
        finder.add_edge(2, 3);
        assert((finder.find() == std::vector<int>{2}));
        finder.add_edge(1, 3);
        assert(finder.find().empty());
    }
    {
        Dinic flow(1);
        assert(flow.flow(1, 1) == 0);
        ScalingMCMF cost_flow(1);
        assert(cost_flow.flow(1, 1) == std::make_pair(0LL, 0LL));
    }
    return 0;
}
