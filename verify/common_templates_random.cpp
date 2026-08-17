#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "../ds/disjoint_set_union.cpp"
#include "../ds/fenwick_tree.cpp"
#include "../graph/kruskal.cpp"
#include "../graph/topological_sort.cpp"
#include "../string/KMP.cpp"
#include "../string/Manacher.cpp"

int main() {
    std::mt19937 rng(114514);
    {
        const int n = 50;
        FenwickTree fenwick(n);
        std::vector<long long> values(n + 1);
        for (int operation = 0; operation < 5000; ++operation) {
            if (rng() % 2 == 0) {
                int index = int(rng() % n) + 1;
                int delta = int(rng() % 101) - 50;
                values[index] += delta;
                fenwick.add(index, delta);
            } else {
                int left = int(rng() % n) + 1, right = int(rng() % n) + 1;
                if (left > right) std::swap(left, right);
                long long expected = std::accumulate(values.begin() + left,
                                                     values.begin() + right + 1, 0LL);
                assert(fenwick.range_sum(left, right) == expected);
            }
        }
    }
    {
        DisjointSetUnion dsu(5);
        assert(dsu.unite(1, 2));
        assert(dsu.unite(2, 3));
        assert(!dsu.unite(1, 3));
        assert(dsu.same(1, 3) && dsu.component_size(2) == 3 && dsu.components == 3);
    }
    {
        for (int test = 0; test < 300; ++test) {
            int n = int(rng() % 7) + 1;
            std::vector<KruskalEdge> edges;
            std::vector<std::vector<long long>> weight(n + 1,
                std::vector<long long>(n + 1, 1LL << 50));
            int id = 0;
            for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
                if (rng() % 3 != 0) {
                    long long w = int(rng() % 31) - 15;
                    edges.push_back({u, v, ++id, w});
                    weight[u][v] = weight[v][u] = std::min(weight[u][v], w);
                }
            KruskalResult result = kruskal_mst(n, edges);

            std::vector<long long> distance(n + 1, 1LL << 50);
            std::vector<char> used(n + 1);
            distance[1] = 0;
            long long prim_weight = 0;
            bool connected = true;
            for (int step = 0; step < n; ++step) {
                int u = 0;
                for (int v = 1; v <= n; ++v)
                    if (!used[v] && (!u || distance[v] < distance[u])) u = v;
                if (!u || distance[u] == (1LL << 50)) {
                    connected = false;
                    break;
                }
                used[u] = 1;
                prim_weight += distance[u];
                for (int v = 1; v <= n; ++v)
                    distance[v] = std::min(distance[v], weight[u][v]);
            }
            assert(result.connected == connected);
            if (connected) assert(result.total_weight == prim_weight);
        }
    }
    {
        std::vector<std::vector<int>> graph(5);
        graph[1] = {2, 3}; graph[2] = {4}; graph[3] = {4};
        auto order = topological_sort(graph);
        assert(order.has_value());
        std::vector<int> position(5);
        for (int i = 0; i < int(order->size()); ++i) position[(*order)[i]] = i;
        for (int u = 1; u <= 4; ++u)
            for (int v : graph[u]) assert(position[u] < position[v]);
        graph[4].push_back(1);
        assert(!topological_sort(graph).has_value());
    }
    {
        for (int test = 0; test < 1000; ++test) {
            std::string text(rng() % 25, 'a'), pattern(rng() % 8, 'a');
            for (char &ch : text) ch = char('a' + rng() % 3);
            for (char &ch : pattern) ch = char('a' + rng() % 3);
            std::vector<int> expected;
            for (int i = 0; i + int(pattern.size()) <= int(text.size()); ++i)
                if (text.compare(i, pattern.size(), pattern) == 0) expected.push_back(i);
            assert(kmp_search(text, pattern) == expected);
        }
    }
    {
        for (int test = 0; test < 500; ++test) {
            std::string text(rng() % 30, 'a');
            for (char &ch : text) ch = char('a' + rng() % 4);
            Manacher manacher(text);
            int longest = 0;
            for (int left = 0; left < int(text.size()); ++left)
                for (int right = left; right < int(text.size()); ++right) {
                    bool expected = true;
                    for (int i = 0; left + i <= right - i; ++i)
                        if (text[left + i] != text[right - i]) expected = false;
                    assert(manacher.is_palindrome(left, right) == expected);
                    if (expected) longest = std::max(longest, right - left + 1);
                }
            assert(manacher.longest_palindrome().second == longest);
        }
    }
    return 0;
}
