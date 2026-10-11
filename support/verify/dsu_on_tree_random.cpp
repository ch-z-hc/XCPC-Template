#include <cassert>
#include <random>
#include <set>
#include <vector>

#include "../../template/graph/DSUOnTree.cpp"

int main() {
    std::mt19937 rng(712367);
    for (int test = 0; test < 500; ++test) {
        int n = int(rng() % 50) + 1;
        DSUOnTree dsu(n);
        for (int u = 2; u <= n; ++u) {
            int parent = int(rng() % (u - 1)) + 1;
            dsu.add_edge(parent, u);
        }

        int root = int(rng() % n) + 1;
        std::vector<int> color(n + 1);
        for (int u = 1; u <= n; ++u) color[u] = int(rng() % 10);

        std::vector<int> parent(n + 1), order{root};
        for (int i = 0; i < int(order.size()); ++i) {
            int u = order[i];
            for (int v : dsu.g[u]) if (v != parent[u]) {
                parent[v] = u;
                order.push_back(v);
            }
        }

        std::vector<int> expected(n + 1);
        for (int u = 1; u <= n; ++u) {
            std::set<int> colors;
            for (int x = 1; x <= n; ++x) {
                int y = x;
                while (y && y != u) y = parent[y];
                if (y == u) colors.insert(color[x]);
            }
            expected[u] = int(colors.size());
        }

        std::vector<int> frequency(10), answer(n + 1);
        int distinct = 0;
        auto add_vertex = [&](int u) {
            if (frequency[color[u]]++ == 0) ++distinct;
        };
        auto remove_vertex = [&](int u) {
            if (--frequency[color[u]] == 0) --distinct;
        };
        auto answer_subtree = [&](int u) { answer[u] = distinct; };
        dsu.build(root, add_vertex, remove_vertex, answer_subtree);

        assert(answer == expected);
        assert(distinct == 0);
        for (int count : frequency) assert(count == 0);

        std::fill(answer.begin(), answer.end(), 0);
        dsu.build(root, add_vertex, remove_vertex, answer_subtree);
        assert(answer == expected);
        assert(distinct == 0);
    }
    return 0;
}
