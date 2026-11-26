/*
离线处理单点修改和闭区间查询，1-based，增删按值。每次求解前清空统计量，离散化须包含修改值。
块长 B 约为 n^(2/3)，q 次查询、u 次修改，排序 O(q log q)，移动 O(qB + n²/B + un²/B²) 次。
文件尾第一段替换结构体中的 3 个成员声明，示例统计区间不同值个数。值先离散到 1..m，按查询编号读 ans。
*/

#include <bits/stdc++.h>

struct MoModify {
    struct Change {
        int p, before, after;
    };
    struct Query {
        int l, r, t, id;
    };
    int n = 0;
    std::vector<int> initial, current;
    std::vector<Change> changes;
    std::vector<Query> qs;

    void add(int x);
    void del(int x);
    void answer(int id);

    void init(const std::vector<int> &a) {
        initial = current = a;
        n = std::max(0, int(a.size()) - 1);
        changes.clear();
        qs.clear();
    }
    void modify(int p, int x) {
        changes.push_back({p, current[p], x});
        current[p] = x;
    }
    int query(int l, int r) {
        int id = int(qs.size());
        qs.push_back({l, r, int(changes.size()), id});
        return id;
    }
    void solve() {
        int B = std::max(1, int(std::pow(std::max(1, n), 2.0 / 3)));
        auto queries = qs;
        std::sort(queries.begin(), queries.end(), [&](Query a, Query b) {
            int x = a.l / B, y = b.l / B;
            if (x != y) return x < y;
            x = a.r / B;
            y = b.r / B;
            if (x != y) return (a.l / B & 1) ? x > y : x < y;
            return (x & 1) ? a.t > b.t : a.t < b.t;
        });
        std::vector<int> a = initial;
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

/*
std::vector<int> cnt, ans; int kinds = 0;
void add(int x) { if (cnt[x] ++ == 0) ++ kinds; }
void del(int x) { if (-- cnt[x] == 0) -- kinds; }
void answer(int id) { ans[id] = kinds; }
*/

/*
MoModify mo; mo.init(a); mo.modify(p, x); int id = mo.query(l, r);
mo.cnt.assign(m + 1, 0); mo.kinds = 0;
mo.ans.resize(mo.qs.size()); mo.solve();
*/
