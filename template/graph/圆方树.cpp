#include <bits/stdc++.h>

#define V vector

using namespace std;

// 从点双顶点列表构造广义圆方森林；不重复实现 Tarjan。
// 原图顶点 1..n 为圆点，点双 i（0-based）对应方点 n + i + 1。
// 输入须为无自环无向图的完整点双分解，每个分量内顶点不重复。
// 孤立点的单点分量也生成一个方点；所有圆点保留，非割点不压缩。
// 时间、空间 O(n + sum(|bcc[i]|))；无模数要求。
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

// 先用 VBCC::build；收集 vbcc.bcc[i].v 到 blocks。
// 调用：auto tree = block_cut_tree(n, blocks);
