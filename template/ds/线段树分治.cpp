#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
时间为 1..n。在 l 加入、在 d 删除的操作覆盖 [l, d - 1]，一直有效则覆盖 [l, n]。
同一时刻的答案须与操作的施加顺序无关。离开节点时回滚到进入前的快照。
m 个区间共存 O(m log n) 个操作编号，每个副本执行一次，另需遍历 O(n) 个节点。
*/

template < class State >
struct SegDivide {
    int n = 0;
    V < V < int > > tr;

    void init(int n_) {
        n = n_;
        tr.assign(n * 4 + 5, {});
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
        if (n && l <= r) add(1, 1, n, l, r, id);
    }

    void dfs(int p, int l, int r, State &s) {
        auto snap = s.snapshot();
        for (int id : tr[p]) s.apply(id);
        if (l == r) s.answer(l);
        else {
            int mid = (l + r) / 2;
            dfs(p * 2, l, mid, s);
            dfs(p * 2 + 1, mid + 1, r, s);
        }
        s.rollback(snap);
    }

    void solve(State &s) {
        if (n) dfs(1, 1, n, s);
    }
};

/*
// 例：查询各时刻 1 和 3 是否连通。先复制「可撤销并查集」。
struct State {
    RollbackDSU dsu{3};
    V < pair < int, int > > edge = {{1, 2}, {2, 3}};
    int ans[5]{};
    int snapshot() { return dsu.snapshot(); }
    void apply(int id) { auto [u, v] = edge[id]; dsu.merge(u, v); }
    void answer(int t) { ans[t] = dsu.find(1) == dsu.find(3); }
    void rollback(int snap) { dsu.rollback(snap); }
};
State s; SegDivide < State > st; st.init(4);
st.add(1, 3, 0); st.add(2, 4, 1); st.solve(s); // ans[1..4] = 0, 1, 1, 0
*/
