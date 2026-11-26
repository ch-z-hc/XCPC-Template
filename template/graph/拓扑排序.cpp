// 有向图拓扑排序，O(n + m)。有环时返回 false。

#include <bits/stdc++.h>

bool topo(const std::vector<std::vector<int>> &g, std::vector<int> &ans) {
    int n = int(g.size()) - 1;
    std::vector<int> deg(n + 1);
    for (int u = 1; u <= n; u ++)
        for (int v : g[u]) ++ deg[v];

    std::queue<int> q;
    for (int u = 1; u <= n; u ++)
        if (deg[u] == 0) q.push(u);
    ans.clear();
    ans.reserve(n);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ans.push_back(u);
        for (int v : g[u])
            if (-- deg[v] == 0) q.push(v);
    }
    return int(ans.size()) == n;
}

/*
std::vector<int> order;
bool acyclic = topo(g, order);
*/
