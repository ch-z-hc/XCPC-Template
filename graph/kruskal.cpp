#include <bits/stdc++.h>
using namespace std;

// Kruskal 最小生成树；无向图顶点编号 1..n，允许负边权和重边。
// 复杂度 O(m log m)，空间 O(n + m)。图不连通时返回最小生成森林且 connected 为 false。
struct KruskalEdge {
    int from, to, id;
    long long weight;
};

struct KruskalResult {
    long long total_weight = 0;
    vector<int> edge_ids;
    bool connected = false;
};

KruskalResult kruskal_mst(int n, vector<KruskalEdge> edges) {
    sort(edges.begin(), edges.end(), [](const KruskalEdge &first, const KruskalEdge &second) {
        if (first.weight != second.weight) return first.weight < second.weight;
        return first.id < second.id;
    });

    vector<int> parent(n + 1), size(n + 1, 1);
    iota(parent.begin(), parent.end(), 0);
    auto find = [&](auto &&self, int x) -> int {
        return parent[x] == x ? x : parent[x] = self(self, parent[x]);
    };

    KruskalResult result;
    for (const KruskalEdge &edge : edges) {
        int first = find(find, edge.from), second = find(find, edge.to);
        if (first == second) continue;
        if (size[first] < size[second]) swap(first, second);
        parent[second] = first;
        size[first] += size[second];
        result.total_weight += edge.weight;
        result.edge_ids.push_back(edge.id);
        if (int(result.edge_ids.size()) == n - 1) break;
    }
    result.connected = n <= 1 || int(result.edge_ids.size()) == n - 1;
    return result;
}

// Usage:
// auto result = kruskal_mst(n, edges);
