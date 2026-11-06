#include <bits/stdc++.h>

#define V vector

using namespace std;

// 有限无环正常规则公平游戏；g[u] 为一步可达状态，顶点 1-based。
// grundy(g, sg) 成功时写入所有状态的 SG 值并返回 true，终止状态为 0。
// 存在有向环返回 false，此时不要使用 sg。
// 多个互不影响子游戏的 SG 值异或，非零当且仅当先手必胜。
// 递归 DFS，O(n + m) 时间、空间；深链需提高栈限制。
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

// 用法：V < int > sg; if (grundy(g, sg)) { int win = sg[u] ^ sg[v]; }
