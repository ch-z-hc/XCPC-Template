#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
点编号 1..n。先用 snapshot() 记下当前位置，rollback(snap) 撤销之后的所有合并。
合并失败也占一个栈位，只能回到当前历史上的快照。
初始化 O(n)，查找和合并 O(log n)，每撤销一次合并 O(1)。
*/
struct RollbackDSU {
    V < int > fa, siz;
    V < pair < int, int > > stk;

    RollbackDSU(int n = 0) { init(n); }

    void init(int n) {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        iota(fa.begin(), fa.end(), 0);
        stk.clear();
    }

    int find(int u) {
        while (fa[u] != u) u = fa[u];
        return u;
    }

    int snapshot() { return int(stk.size()); }

    bool merge(int u, int v) {
        u = find(u); v = find(v);
        if (u == v) { stk.push_back({-1, -1}); return 0; }
        if (siz[u] < siz[v]) swap(u, v);
        stk.push_back({v, siz[u]});
        fa[v] = u;
        siz[u] += siz[v];
        return 1;
    }

    void rollback(int snap) {
        while (int(stk.size()) > snap) {
            auto [v, old] = stk.back(); stk.pop_back();
            if (v == -1) continue;
            int u = fa[v];
            fa[v] = v;
            siz[u] = old;
        }
    }
};

/*
RollbackDSU dsu(n);
int snap = dsu.snapshot();
dsu.merge(u, v);
dsu.rollback(snap);
*/
