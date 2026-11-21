#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
先用 EBCC::build 求完整边双分解。bel[u] 是点 u 所在分量（1..cnt），edges 是从 0 编号的原边表。
返回桥森林的邻接表，每项为 {相邻分量, 原桥编号}。孤立分量保留，允许重边和自环。
时间 O(n + m)，结果占 O(cnt + 桥数) 空间。
*/
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

/*
ebcc.build();
auto tree = bridge_tree(ebcc.cnt, ebcc.bel, ebcc.edges);
*/
