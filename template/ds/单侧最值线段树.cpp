/*
默认做区间加、逐项 chmin，查询区间最小值。模板参数改为 false 就是区间加、逐项 chmax、查询最大值。
modify(l, r, d, v) 先加 d，再对每项取 min(x, v) 或 max(x, v)。
建树 O(n)，每次修改、查询 O(log n)。a[1..n]，区间为 1-based 闭区间。
元素、有限边界及其加法中间值须严格位于 LLONG_MIN 与 LLONG_MAX 之间，累计偏移须放入 ll。
空区间查询返回最值运算的单位元。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

template < bool is_min = true >
struct AddMinMaxSeg {
    static ll id() { return is_min ? LLONG_MAX : LLONG_MIN; }
    static ll best(ll a, ll b) { return is_min ? min(a, b) : max(a, b); }

    struct Node {
        ll val = 0, add = 0, bound = id();
    };
    int n = 0;
    V < Node > t;

    void pull(int p) { t[p].val = best(t[p * 2].val, t[p * 2 + 1].val); }

    void apply(int p, ll d, ll v) {
        auto &u = t[p];
        u.val = best(u.val + d, v);
        u.add += d;
        if (u.bound != id()) u.bound += d;
        u.bound = best(u.bound, v);
    }

    void push(int p) {
        auto &u = t[p];
        if (!u.add && u.bound == id()) return;
        apply(p * 2, u.add, u.bound);
        apply(p * 2 + 1, u.add, u.bound);
        u.add = 0; u.bound = id();
    }

    void build(int p, int l, int r, const V < ll > &a) {
        if (l == r) {
            t[p].val = a[l];
            return;
        }
        int mid = l + (r - l) / 2;
        build(p * 2, l, mid, a);
        build(p * 2 + 1, mid + 1, r, a);
        pull(p);
    }

    void init(const V < ll > &a) {
        n = max(0, int(a.size()) - 1);
        t.assign(4ull * n + 5, {});
        if (n) build(1, 1, n, a);
    }

    void modify(int p, int l, int r, int x, int y, ll d, ll v) {
        if (x <= l && r <= y) {
            apply(p, d, v);
            return;
        }
        push(p);
        int mid = l + (r - l) / 2;
        if (x <= mid) modify(p * 2, l, mid, x, y, d, v);
        if (y > mid) modify(p * 2 + 1, mid + 1, r, x, y, d, v);
        pull(p);
    }

    ll query(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return t[p].val;
        push(p);
        int mid = l + (r - l) / 2;
        ll ans = id();
        if (x <= mid) ans = best(ans, query(p * 2, l, mid, x, y));
        if (y > mid) ans = best(ans, query(p * 2 + 1, mid + 1, r, x, y));
        return ans;
    }

    void modify(int l, int r, ll d, ll v) {
        if (n && l <= r) modify(1, 1, n, l, r, d, v);
    }
    void add(int l, int r, ll d) { modify(l, r, d, id()); }
    void chmin(int l, int r, ll v) {
        static_assert(is_min);
        modify(l, r, 0, v);
    }
    void chmax(int l, int r, ll v) {
        static_assert(!is_min);
        modify(l, r, 0, v);
    }
    ll query(int l, int r) { return n && l <= r ? query(1, 1, n, l, r) : id(); }
};

/*
AddMinMaxSeg <> mn; mn.init({0, 3, 1, 4});
mn.add(1, 2, 2); mn.chmin(1, 3, 4);
mn.modify(1, 3, -1, 2);
ll low = mn.query(1, 3);

AddMinMaxSeg < false > mx; mx.init({0, 3, 1, 4});
mx.add(1, 2, -2); mx.chmax(1, 3, 2);
mx.modify(1, 3, 1, 5);
ll high = mx.query(1, 3);
*/
