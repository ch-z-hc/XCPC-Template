/*
按快照撤销合并，只能回到当前历史上的快照，失败的合并也占一个栈位。
初始化 O(n)，查找和合并 O(log n)，每撤销一次合并 O(1)。
*/

#include <bits/stdc++.h>

struct RollbackDSU {
    std::vector<int> fa, siz;
    std::vector<int> stk;

    RollbackDSU(int n = 0) { init(n); }

    void init(int n) {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        std::iota(fa.begin(), fa.end(), 0);
        stk.clear();
    }

    int find(int u) {
        while (fa[u] != u) u = fa[u];
        return u;
    }

    int snapshot() { return int(stk.size()); }

    bool merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) {
            stk.push_back(-1);
            return 0;
        }
        if (siz[u] < siz[v]) std::swap(u, v);
        stk.push_back(v);
        fa[v] = u;
        siz[u] += siz[v];
        return 1;
    }

    void rollback(int snap) {
        while (int(stk.size()) > snap) {
            int v = stk.back();
            stk.pop_back();
            if (v == -1) continue;
            int u = fa[v];
            fa[v] = v;
            siz[u] -= siz[v];
        }
    }
};

/*
RollbackDSU dsu; dsu.init(3); int snap = dsu.snapshot();
bool merged = dsu.merge(1, 2); int root = dsu.find(2);
dsu.rollback(snap);
*/
