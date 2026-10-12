#include <bits/stdc++.h>

#define V vector

using namespace std;

// 离线动态连通性：顶点 1..n，操作按时间记录，时间线段树 + 可撤销 DSU。
// add / erase 支持自环与重边；erase 必须存在，每次只删一份。
// query(u, v) 返回答案编号；solve 返回各查询的 0 / 1，可重复求解。
// q 次操作时间 O(q log q log n)，空间 O(n + q log q)；不含路径压缩。
struct DynConn {
    struct Op { int type, u, v; };
    int n;
    V < Op > ops;
    V < int > fa, siz, ans;
    V < pair < int, int > > stk;
    V < V < pair < int, int > > > tr;

    DynConn(int n = 0) { init(n); }
    void init(int n_) { n = n_; ops.clear(); ans.clear(); }
    void add(int u, int v) { ops.push_back({0, min(u, v), max(u, v)}); }
    void erase(int u, int v) { ops.push_back({1, min(u, v), max(u, v)}); }
    int query(int u, int v) {
        int id = int(ans.size()); ans.push_back(0);
        ops.push_back({2, u, v});
        return id;
    }

    int find(int u) { return fa[u] == u ? u : find(fa[u]); }
    void merge(int u, int v) {
        u = find(u); v = find(v);
        if (u == v) return;
        if (siz[u] < siz[v]) swap(u, v);
        stk.push_back({v, siz[u]}); fa[v] = u; siz[u] += siz[v];
    }
    void rollback(int snap) {
        while (int(stk.size()) > snap) {
            auto [v, s] = stk.back(); stk.pop_back();
            siz[fa[v]] = s; fa[v] = v;
        }
    }

    void insert(int p, int l, int r, int x, int y, pair < int, int > edge) {
        if (x > r || y < l) return;
        if (x <= l && r <= y) { tr[p].push_back(edge); return; }
        int mid = (l + r) >> 1;
        insert(p << 1, l, mid, x, y, edge);
        insert(p << 1 | 1, mid + 1, r, x, y, edge);
    }

    void dfs(int p, int l, int r, int &id) {
        int snap = int(stk.size());
        for (auto [u, v] : tr[p]) merge(u, v);
        if (l == r) {
            if (ops[l].type == 2) ans[id ++] = find(ops[l].u) == find(ops[l].v);
        } else {
            int mid = (l + r) >> 1;
            dfs(p << 1, l, mid, id); dfs(p << 1 | 1, mid + 1, r, id);
        }
        rollback(snap);
    }

    V < int > solve() {
        int q = int(ops.size());
        tr.assign(4 * q + 5, {});
        map < pair < int, int >, V < int > > active;
        for (int i = 0; i < q; ++ i) {
            auto [type, u, v] = ops[i];
            if (type == 0) active[{u, v}].push_back(i);
            else if (type == 1) {
                auto &a = active[{u, v}];
                int l = a.back(); a.pop_back();
                insert(1, 0, q - 1, l, i - 1, {u, v});
            }
        }
        for (auto &[edge, a] : active)
            for (int l : a) insert(1, 0, q - 1, l, q - 1, edge);
        fa.resize(n + 1); iota(fa.begin(), fa.end(), 0);
        siz.assign(n + 1, 1); stk.clear();
        int id = 0;
        if (q) dfs(1, 0, q - 1, id);
        return ans;
    }
};

// DynConn dc(n); dc.add(u, v); int id = dc.query(u, v);
// dc.erase(u, v); auto ans = dc.solve();
