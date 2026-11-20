#include <bits/stdc++.h>

#define V vector

using namespace std;

// 先求无自环无向图的完整点双分解，把每个 vbcc.bcc[i].v 放入 blocks，分量内的顶点不能重复。
// 圆点沿用原编号 1..n，第 i 个点双对应方点 n + i + 1（i 从 0 数）。孤立点也连一个方点。
// 构造时间和空间 O(n + 各点双大小之和)。
V < V < int > > block_cut_tree(int n, const V < V < int > > &bcc) {
    V < V < int > > tree(n + bcc.size() + 1);
    for (int i = 0; i < int(bcc.size()); ++ i) {
        int p = n + i + 1;
        for (int u : bcc[i]) {
            tree[u].push_back(p); tree[p].push_back(u);
        }
    }
    return tree;
}

// V < V < int > > blocks;
// for (auto &b : vbcc.bcc) blocks.push_back(b.v);
// auto tree = block_cut_tree(n, blocks);
