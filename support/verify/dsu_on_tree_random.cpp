#include <bits/stdc++.h>

#include "../../template/graph/树上启发式合并.cpp"

std::vector<int> color, frequency, answer;
int distinct = 0;

void DSUOnTree::add(int u) {
    if (frequency[color[u]]++ == 0) ++distinct;
}
void DSUOnTree::del(int u) {
    assert(frequency[color[u]] > 0);
    if (--frequency[color[u]] == 0) --distinct;
}
void DSUOnTree::query(int u) { answer[u] = distinct; }

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
        color.assign(n + 1, 0);
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

        frequency.assign(10, 0); answer.assign(n + 1, 0);
        distinct = 0;
        dsu.build(root);

        assert(answer == expected);
        assert(distinct == 0);
        for (int count : frequency) assert(count == 0);

        std::fill(answer.begin(), answer.end(), 0);
        dsu.build(root);
        assert(answer == expected);
        assert(distinct == 0);
    }
    return 0;
}
