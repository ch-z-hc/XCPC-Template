#include <bits/stdc++.h>

#define V vector

using namespace std;

// 有向图拓扑排序；g 使用 1-based 顶点编号，存在环时返回 false。
// 成功返回 true，将拓扑序写入 ans；失败时不要使用 ans。
// 复杂度 O(n + m)，空间 O(n)。

bool topo(const V < V < int > > &g, V < int > &ans) {
    int n = int(g.size()) - 1;
    V < int > deg(n + 1);
    for (int u = 1; u <= n; ++ u)
        for (int v : g[u]) ++ deg[v];

    queue < int > q;
    for (int u = 1; u <= n; ++ u) if (deg[u] == 0) q.push(u);
    ans.clear();
    ans.reserve(n);
    while (! q.empty()) {
        int u = q.front();
        q.pop();
        ans.push_back(u);
        for (int v : g[u]) if (-- deg[v] == 0) q.push(v);
    }
    return int(ans.size()) == n;
}

// 调用示例：
// V < int > ans;
// if (topo(g, ans)) for (int u : ans) { /* 拓扑序。 */ }
