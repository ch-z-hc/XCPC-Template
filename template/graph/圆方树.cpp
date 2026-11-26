/*
由无自环无向图的完整点双分量构造圆方森林，O(n + 各点双大小之和)。
分量内顶点不能重复。圆点编号 1..n，第 i 个点双对应方点 n + i + 1，i 从 0 数。
*/

#include <bits/stdc++.h>

std::vector<std::vector<int>> block_cut_tree(int n,
                                             const std::vector<std::vector<int>> &bcc) {
    std::vector<std::vector<int>> tree(n + bcc.size() + 1);
    for (int i = 0; i < int(bcc.size()); i ++) {
        int p = n + i + 1;
        for (int u : bcc[i]) {
            tree[u].push_back(p);
            tree[p].push_back(u);
        }
    }
    return tree;
}

/*
std::vector<std::vector<int>> blocks;
for (auto &b : vbcc.bcc) blocks.push_back(b.v);
auto tree = block_cut_tree(n, blocks);
*/
