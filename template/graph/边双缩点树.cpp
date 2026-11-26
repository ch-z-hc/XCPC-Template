/*
由完整的边双分量构造桥森林，O(n + m)。bel[u] 从 1 编号，原边按输入顺序从 0 编号。
邻接表保留原桥编号和孤立分量，允许重边、自环。
*/

#include <bits/stdc++.h>

std::vector<std::vector<std::pair<int, int>>>
bridge_tree(int cnt, const std::vector<int> &bel,
            const std::vector<std::pair<int, int>> &edges) {
    std::vector<std::vector<std::pair<int, int>>> tree(cnt + 1);
    for (int i = 0; i < int(edges.size()); i ++) {
        auto [u, v] = edges[i];
        u = bel[u];
        v = bel[v];
        if (u == v) continue;
        tree[u].push_back({v, i});
        tree[v].push_back({u, i});
    }
    return tree;
}

/*
ebcc.build();
auto tree = bridge_tree(ebcc.cnt, ebcc.bel, ebcc.edges);
*/
