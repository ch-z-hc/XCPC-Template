/*
求无环公平游戏的 SG 值，O(n + m)。状态从 1 编号，无法行动者输，终止状态为 0。
有环则失败；独立子游戏的 SG 异或和非零时先手胜。长链注意递归栈。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

bool grundy(const V < V < int > > &g, V < int > &sg) {
    int n = int(g.size()) - 1;
    V < int > color(n + 1, 0);
    sg.assign(n + 1, 0);
    auto dfs = [&](auto &&self, int u) -> bool {
        if (color[u]) return color[u] == 2;
        color[u] = 1;
        for (int v : g[u]) if (! self(self, v)) return false;
        V < bool > seen(g[u].size() + 1, false);
        for (int v : g[u]) if (sg[v] < int(seen.size())) seen[sg[v]] = true;
        while (seen[sg[u]]) ++ sg[u];
        color[u] = 2;
        return true;
    };
    for (int u = 1; u <= n; ++ u) if (! dfs(dfs, u)) return false;
    return true;
}

/*
V < int > sg;
bool ok = grundy(g, sg);
*/
