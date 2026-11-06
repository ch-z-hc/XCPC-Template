#include <bits/stdc++.h>

#define V vector

using namespace std;

// 从边双编号构造桥森林，分量编号 1..cnt。
// bel 为原图 1-based 顶点对应的边双编号，edges 为 0-based 原边列表。
// 返回邻接项 {相邻分量, 原桥编号}，保留孤立分量；支持重边和自环。
// 输入须是完整边双分解；时间 O(n + m)，空间 O(cnt + 桥数)。
V < V < pair < int, int > > > bridge_tree(int cnt, const V < int > &bel,
                                          const V < pair < int, int > > &edges) {
    V < V < pair < int, int > > > tree(cnt + 1);
    for (int i = 0; i < int(edges.size()); ++ i) {
        auto [u, v] = edges[i];
        u = bel[u]; v = bel[v];
        if (u == v) continue;
        tree[u].push_back({v, i}); tree[v].push_back({u, i});
    }
    return tree;
}

// 先用 EBCC::build。
// 用法：auto tree = bridge_tree(ebcc.cnt, ebcc.bel, ebcc.edges);
