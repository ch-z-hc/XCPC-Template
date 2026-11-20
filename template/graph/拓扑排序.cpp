#include <bits/stdc++.h>

#define V vector

using namespace std;

// 有向图顶点编号 1..n。topo(g, ans) 将拓扑序写入 ans，有环时返回 false。
// 时间 O(n + m)，额外空间 O(n)。

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

// V < int > order;
// bool acyclic = topo(g, order);
