#include <bits/stdc++.h>

#define V vector

using namespace std;

// 带修改莫队；数组 1-based，查询闭区间；按时间记录 modify / query。
// add(value)、del(value) 维护当前区间，answer(id) 保存答案。
// solve 前回调状态必须为空；solve 不消耗操作，可重跑但要重置回调状态。
// 块长 B ≈ n^(2/3)，q 次查询、u 次修改：排序 O(q log q)。
// 移动 O(q B + n^2 / B + u n^2 / B^2)，空间 O(n + q + u)。
struct MoModify {
    struct Change { int p, before, after; };
    struct Query { int l, r, t, id; };
    int n = 0;
    V < int > initial, current;
    V < Change > changes;
    V < Query > qs;

    void init(const V < int > &a) {
        initial = current = a; n = max(0, int(a.size()) - 1);
        changes.clear(); qs.clear();
    }
    void modify(int p, int x) {
        changes.push_back({p, current[p], x}); current[p] = x;
    }
    int query(int l, int r) {
        int id = int(qs.size()); qs.push_back({l, r, int(changes.size()), id});
        return id;
    }
    template < class Add, class Remove, class Answer >
    void solve(Add add, Remove del, Answer answer) const {
        int B = max(1, int(pow(max(1, n), 2.0 / 3)));
        auto queries = qs;
        sort(queries.begin(), queries.end(), [&](Query a, Query b) {
            int x = a.l / B, y = b.l / B;
            if (x != y) return x < y;
            x = a.r / B; y = b.r / B;
            if (x != y) return (a.l / B & 1) ? x > y : x < y;
            return (x & 1) ? a.t > b.t : a.t < b.t;
        });
        V < int > a = initial;
        int l = 1, r = 0, t = 0;
        auto apply = [&](int id, bool forward) {
            auto c = changes[id];
            if (l <= c.p && c.p <= r) del(a[c.p]);
            a[c.p] = forward ? c.after : c.before;
            if (l <= c.p && c.p <= r) add(a[c.p]);
        };
        for (auto q : queries) {
            while (t < q.t) apply(t ++, true);
            while (t > q.t) apply(-- t, false);
            while (l > q.l) add(a[-- l]);
            while (r < q.r) add(a[++ r]);
            while (l < q.l) del(a[l ++]);
            while (r > q.r) del(a[r --]);
            answer(q.id);
        }
    }
};

// MoModify mo; mo.init(a); mo.modify(p, x); int id = mo.query(l, r);
// mo.solve(add, del, answer);
