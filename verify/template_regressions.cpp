#include <cassert>
#include <vector>

#include "../ds/LiChao_SegT.cpp"
#include "../graph/CutEdge.cpp"
#include "../graph/CutVertex.cpp"
#include "../graph/MaxFlow.cpp"
#include "../graph/MaxFlowMinMaxCost.cpp"
#include "../math/bm.cpp"
#include "../string/AC-automaton.cpp"

int main() {
    {
        LiChaoTree tree(4);
        int id = tree.add_line(0, -1e20L);
        assert(tree.query_line(2) == id);
        assert(tree.query(2) == -1e20L);
    }
    {
        const long long mod = 1000000007;
        assert(linear_recurrence_kth({3}, {2}, 5, mod) == 96);
    }
    {
        AhoCorasick ac;
        ac.add_pattern("a");
        ac.build();
        assert((ac.count_occurrences("aa") == std::vector<long long>{2}));
        ac.build();
        ac.add_pattern("aa");
        ac.build();
        assert((ac.count_occurrences("aa") == std::vector<long long>{2, 1}));
    }
    {
        BridgeFinder finder(2);
        finder.add_edge(1, 1, 2);
        assert((finder.find() == std::vector<int>{1}));
        finder.add_edge(2, 1, 2);
        assert(finder.find().empty());
    }
    {
        ArticulationFinder finder(3);
        finder.add_edge(1, 2);
        finder.add_edge(2, 3);
        assert((finder.find() == std::vector<int>{2}));
        finder.add_edge(1, 3);
        assert(finder.find().empty());
    }
    {
        Dinic flow(1);
        assert(flow.max_flow(1, 1) == 0);
        MinCostMaxFlow cost_flow(1);
        assert(cost_flow.flow(1, 1) == std::make_pair(0LL, 0LL));
    }
    return 0;
}
