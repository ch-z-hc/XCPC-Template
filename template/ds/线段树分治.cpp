/*
时间为 1..q。在 l 加入、在 d 删除的操作覆盖 [l, d - 1]，一直有效则覆盖 [l, q]。
同一时刻的答案须与操作的施加顺序无关。m 个区间共存 O(m log q) 个操作编号。
动态连通性示例先复制「可撤销并查集」，总时间 O(n + (m log q + q) log n)。
换题时直接改全局变量和 dfs。
*/

#include <bits/stdc++.h>

#define V vector
#define pii pair < int, int >

using namespace std;

int q;
V < V < int > > tr;
RollbackDSU dsu;
V < pii > edge, ask;
V < int > ans;

void init(int n, int q_) {
    q = q_;
    tr.assign(q * 4 + 5, {});
    dsu.init(n);
    edge.clear();
    ask.assign(q + 1, {0, 0});
    ans.assign(q + 1, 0);
}

void add(int p, int l, int r, int x, int y, int id) {
    if (x <= l && r <= y) {
        tr[p].push_back(id);
        return;
    }
    int mid = (l + r) / 2;
    if (x <= mid) add(p * 2, l, mid, x, y, id);
    if (y > mid) add(p * 2 + 1, mid + 1, r, x, y, id);
}

void add(int l, int r, int id) {
    if (q && l <= r) add(1, 1, q, l, r, id);
}

void dfs(int p, int l, int r) {
    if (l > r) return;
    int snap = dsu.snapshot();
    for (int id : tr[p]) {
        auto [u, v] = edge[id];
        dsu.merge(u, v);
    }
    if (l == r) {
        auto [u, v] = ask[l];
        if (u) ans[l] = dsu.find(u) == dsu.find(v);
    } else {
        int mid = (l + r) / 2;
        dfs(p * 2, l, mid);
        dfs(p * 2 + 1, mid + 1, r);
    }
    dsu.rollback(snap);
}

/*
init(3, 4);
edge = {{1, 2}, {2, 3}};
add(1, 3, 0); add(2, 4, 1);
for (int t = 1; t <= q; t ++) ask[t] = {1, 3};
dfs(1, 1, q);
*/
