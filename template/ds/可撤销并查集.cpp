#include <bits/stdc++.h>

#define V vector

using namespace std;

// 可撤销并查集；编号为 1..n，按大小合并，不使用路径压缩。
// 初始化 O(n)，find 和 merge 为 O(log n)，撤销每次合并 O(1)。
// 注意 snapshot 记录操作栈长度，合并失败也占一个记录；仅能回滚到已有快照。
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

// 调用示例：
// RollbackDSU dsu(n);
// int snap = dsu.snapshot();
// dsu.merge(u, v);
// dsu.rollback(snap);
