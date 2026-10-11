#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "../../template/ds/DSU.cpp"
#include "../../template/ds/Fenwick.cpp"
#include "../../template/ds/SegTree.cpp"
#include "../../template/graph/Dijkstra.cpp"
#include "../../template/graph/Kruskal.cpp"
#include "../../template/graph/TopologicalSort.cpp"
#include "../../template/string/KMP.cpp"
#include "../../template/string/Manacher.cpp"

int main() {
    std::mt19937 rng(114514);
    {
        const int n = 50;
        Fenwick fenwick(n);
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
                assert(fenwick.query(left, right) == expected);
            }
        }
    }
    {
        DSU dsu(5);
        assert(dsu.merge(1, 2));
        assert(dsu.merge(2, 3));
        assert(!dsu.merge(1, 3));
        assert(dsu.same(1, 3) && dsu.size(2) == 3 && dsu.cnt == 3);

        for (int test = 0; test < 200; ++test) {
            int n = int(rng() % 30) + 1, components = n;
            dsu.init(n);
            std::vector<int> label(n + 1);
            std::iota(label.begin(), label.end(), 0);
            for (int operation = 0; operation < 300; ++operation) {
                int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
                if (rng() % 2 == 0) {
                    bool merged = label[u] != label[v];
                    assert(dsu.merge(u, v) == merged);
                    if (merged) {
                        int from = label[v], to = label[u];
                        for (int i = 1; i <= n; ++i)
                            if (label[i] == from) label[i] = to;
                        --components;
                    }
                }
                assert(dsu.same(u, v) == (label[u] == label[v]));
                int size = int(std::count(label.begin() + 1, label.end(), label[u]));
                assert(dsu.size(u) == size);
                assert(dsu.cnt == components);
            }
        }
        dsu.init(0);
        assert(dsu.cnt == 0);
        dsu.init(1);
        assert(dsu.find(1) == 1 && dsu.size(1) == 1);
    }
    {
        SegTree st;
        for (int test = 0; test < 200; ++test) {
            int n = int(rng() % 41);
            std::vector<long long> values(n + 1);
            values[0] = 123;
            for (int i = 1; i <= n; ++i) values[i] = int(rng() % 2000001) - 1000000;
            st.init(values);
            assert(st.query(2, 1) == 0);
            st.modify(2, 1, 7);
            if (n == 0) {
                st.modify(1, 0, 5);
                assert(st.query(1, 0) == 0);
                continue;
            }
            for (int operation = 0; operation < 500; ++operation) {
                int left = int(rng() % n) + 1, right = int(rng() % n) + 1;
                if (left > right) std::swap(left, right);
                if (operation % 11 == 0) { left = 1; right = n; }
                if (operation % 13 == 0) right = left;
                if (rng() % 2 == 0) {
                    long long delta = int(rng() % 2000001) - 1000000;
                    st.modify(left, right, delta);
                    for (int i = left; i <= right; ++i) values[i] += delta;
                } else {
                    long long expected = std::accumulate(values.begin() + left,
                                                         values.begin() + right + 1, 0LL);
                    assert(st.query(left, right) == expected);
                }
            }
        }
    }
    {
        const long long inf = 1LL << 50;
        Graph graph(5);
        graph[1] = {{2, 3000000000000LL}, {3, 9000000000000LL}};
        graph[2] = {{3, 4000000000000LL}};
        auto distance = dijkstra(graph, 1);
        assert(distance[3] == 7000000000000LL && distance[4] == (1LL << 62));

        for (int test = 0; test < 250; ++test) {
            int n = int(rng() % 12) + 1;
            graph.assign(n + 1, {});
            std::vector<std::vector<long long>> expected(n + 1,
                std::vector<long long>(n + 1, inf));
            for (int i = 1; i <= n; ++i) expected[i][i] = 0;
            int m = int(rng() % (2 * n * n + 1));
            for (int i = 0; i < m; ++i) {
                int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
                long long weight = rng() % 1001;
                graph[u].push_back({v, weight});
                expected[u][v] = std::min(expected[u][v], weight);
            }
            for (int k = 1; k <= n; ++k)
                for (int u = 1; u <= n; ++u)
                    for (int v = 1; v <= n; ++v)
                        if (expected[u][k] != inf && expected[k][v] != inf)
                            expected[u][v] = std::min(expected[u][v],
                                                     expected[u][k] + expected[k][v]);
            for (int source = 1; source <= n; ++source) {
                distance = dijkstra(graph, source, inf);
                for (int v = 1; v <= n; ++v) assert(distance[v] == expected[source][v]);
            }
        }
    }
    {
        for (int test = 0; test < 300; ++test) {
            int n = int(rng() % 7) + 1;
            std::vector<Edge> edges;
            std::vector<std::vector<long long>> weight(n + 1,
                std::vector<long long>(n + 1, 1LL << 50));
            int id = 0;
            for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
                if (rng() % 3 != 0) {
                    long long w = int(rng() % 31) - 15;
                    edges.push_back({u, v, ++id, w});
                    weight[u][v] = weight[v][u] = std::min(weight[u][v], w);
                }
            MST result = kruskal(n, edges);

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
            assert(result.ok == connected);
            if (connected) assert(result.sum == prim_weight);
        }
    }
    {
        std::vector<std::vector<int>> graph(5);
        graph[1] = {2, 3}; graph[2] = {4}; graph[3] = {4};
        auto order = topo(graph);
        assert(order.has_value());
        std::vector<int> position(5);
        for (int i = 0; i < int(order->size()); ++i) position[(*order)[i]] = i;
        for (int u = 1; u <= 4; ++u)
            for (int v : graph[u]) assert(position[u] < position[v]);
        graph[4].push_back(1);
        assert(!topo(graph).has_value());
    }
    {
        for (int test = 0; test < 1000; ++test) {
            std::string text(rng() % 25, 'a'), pattern(rng() % 8, 'a');
            for (char &ch : text) ch = char('a' + rng() % 3);
            for (char &ch : pattern) ch = char('a' + rng() % 3);
            std::vector<int> expected;
            for (int i = 0; i + int(pattern.size()) <= int(text.size()); ++i)
                if (text.compare(i, pattern.size(), pattern) == 0) expected.push_back(i);
            assert(kmp(text, pattern) == expected);
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
                    assert(manacher.query(left, right) == expected);
                    if (expected) longest = std::max(longest, right - left + 1);
                }
            assert(manacher.longest().second == longest);
        }
    }
    return 0;
}
