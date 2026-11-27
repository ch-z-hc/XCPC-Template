// ok 表示图是否连通，sum、ids 分别为选中边的总权和编号。

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct Edge {
    int u, v, id;
    ll w;
};

struct MST {
    ll sum = 0;
    V < int > ids;
    bool ok = false;
};

MST kruskal(int n, V < Edge > edges) {
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        if (a.w != b.w) return a.w < b.w;
        return a.id < b.id;
    });

    V < int > fa(n + 1), siz(n + 1, 1);
    iota(fa.begin(), fa.end(), 0);
    auto find = [&](auto &&self, int x) -> int {
        return fa[x] == x ? x : fa[x] = self(self, fa[x]);
    };

    MST ans;
    for (const Edge &e : edges) {
        int a = find(find, e.u), b = find(find, e.v);
        if (a == b) continue;
        if (siz[a] < siz[b]) swap(a, b);
        fa[b] = a;
        siz[a] += siz[b];
        ans.sum += e.w;
        ans.ids.push_back(e.id);
        if (int(ans.ids.size()) == n - 1) break;
    }
    ans.ok = n <= 1 || int(ans.ids.size()) == n - 1;
    return ans;
}

/*
V < Edge > edges; edges.push_back({u, v, id, w});
auto mst = kruskal(n, edges);
*/
