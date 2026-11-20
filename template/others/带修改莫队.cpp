#include <bits/stdc++.h>

#define V vector

using namespace std;

// 按时间顺序记录单点修改和闭区间查询，数组从 1 数。add 和 del 接收元素值，answer(id) 保存答案。
// 每次 solve 前清空自己的统计量，操作记录可重复用。统计不同值时，初始值和修改值要一起离散化。
// 块长 B 约为 n^(2/3)。q 次查询、u 次修改，排序 O(q log q)，移动 O(qB + n²/B + un²/B²) 次。
// 空间 O(n + q + u)，另加题目的统计量。
struct MoModify {
    struct Change { int p, before, after; };
    struct Query { int l, r, t, id; };
    int n = 0;
    V < int > initial, current;
    V < Change > changes;
    V < Query > qs;

    // 示例维护：用以下成员替换下方 3 个函数声明。
    // V < int > cnt, ans; int kinds = 0;
    // void add(int x) { if (cnt[x] ++ == 0) ++ kinds; }
    // void del(int x) { if (-- cnt[x] == 0) -- kinds; }
    // void answer(int id) { ans[id] = kinds; }
    void add(int x);
    void del(int x);
    void answer(int id);

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
    void solve() {
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
// mo.cnt.assign(m + 1, 0); mo.kinds = 0; // 值已离散到 1..m
// mo.ans.resize(mo.qs.size()); mo.solve(); // 读 mo.ans[id]
