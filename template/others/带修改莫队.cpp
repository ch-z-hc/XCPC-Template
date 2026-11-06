#include <bits/stdc++.h>

#define V vector

using namespace std;

// 带修改莫队；数组 1-based，查询闭区间；按时间记录 modify / query。
// 使用前补全 add / del / answer 的定义及题目状态。
// add(value)、del(value) 维护当前区间，answer(id) 保存答案。
// solve 不消耗操作记录；每次求解前清空统计状态。
// 块长 B ≈ n^(2/3)，q 次查询、u 次修改：排序 O(q log q)。
// 移动 O(q B + n^2 / B + u n^2 / B^2)，空间 O(n + q + u)。
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

// 用法：MoModify mo; mo.init(a); mo.modify(p, x); int id = mo.query(l, r);
// 统计不同值个数；初始值和所有修改值一起离散到 [1, m]。
// mo.cnt.assign(m + 1, 0); mo.kinds = 0; mo.ans.resize(mo.qs.size());
// mo.solve(); // 第 id 个查询的答案为 mo.ans[id]。
// 重跑前清零 cnt 和 kinds。
