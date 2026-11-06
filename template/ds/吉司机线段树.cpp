#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 吉司机线段树：区间加、取 min / max、求和、最小值和最大值。
// a[1..n]，闭区间 [l, r]；操作要求非空数组且下标有效。
// 建树 O(n)，完整操作序列总摊还 O((n + q) log^2 n)，空间 O(n)。
// 所有元素和截断值严格位于 (-INF, INF)，INF = 2^60。
// 差值乘计数、区间和、累积加标记等中间运算均须不溢出 ll。
// init 可重建，a[0] 为占位；init({0}) 可建立空结构。
struct Beats {
    static const ll INF = 1LL << 60;
    struct Node {
        ll sum = 0, max1 = -INF, max2 = -INF;
        ll min1 = INF, min2 = INF, add = 0;
        int cntmax = 0, cntmin = 0;
    };
    int n = 0;
    V < Node > t;

    void pull(int p) {
        auto &u = t[p];
        auto &a = t[p << 1]; auto &b = t[p << 1 | 1];
        u.sum = a.sum + b.sum;
        u.max1 = max(a.max1, b.max1);
        u.cntmax = (a.max1 == u.max1 ? a.cntmax : 0)
                 + (b.max1 == u.max1 ? b.cntmax : 0);
        u.max2 = max(a.max1 == u.max1 ? a.max2 : a.max1,
                     b.max1 == u.max1 ? b.max2 : b.max1);
        u.min1 = min(a.min1, b.min1);
        u.cntmin = (a.min1 == u.min1 ? a.cntmin : 0)
                 + (b.min1 == u.min1 ? b.cntmin : 0);
        u.min2 = min(a.min1 == u.min1 ? a.min2 : a.min1,
                     b.min1 == u.min1 ? b.min2 : b.min1);
    }

    void apply_add(int p, int len, ll x) {
        auto &u = t[p];
        u.sum += x * len;
        u.max1 += x; u.min1 += x; u.add += x;
        if (u.max2 != -INF) u.max2 += x;
        if (u.min2 != INF) u.min2 += x;
    }

    // 调用前保证 max2 < x < max1，只修改最大值一层。
    void apply_min(int p, ll x) {
        auto &u = t[p];
        u.sum += (x - u.max1) * u.cntmax;
        if (u.min1 == u.max1) u.min1 = x;
        else if (u.min2 == u.max1) u.min2 = x;
        u.max1 = x;
    }

    // 调用前保证 min1 < x < min2，只修改最小值一层。
    void apply_max(int p, ll x) {
        auto &u = t[p];
        u.sum += (x - u.min1) * u.cntmin;
        if (u.max1 == u.min1) u.max1 = x;
        else if (u.max2 == u.min1) u.max2 = x;
        u.min1 = x;
    }

    void push(int p, int l, int r) {
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (t[p].add) {
            apply_add(p << 1, mid - l + 1, t[p].add);
            apply_add(p << 1 | 1, r - mid, t[p].add);
            t[p].add = 0;
        }
        for (int v : {p << 1, p << 1 | 1}) {
            if (t[v].max1 > t[p].max1) apply_min(v, t[p].max1);
            if (t[v].min1 < t[p].min1) apply_max(v, t[p].min1);
        }
    }

    void build(int p, int l, int r, const V < ll > &a) {
        if (l == r) {
            t[p].sum = t[p].min1 = t[p].max1 = a[l];
            t[p].cntmin = t[p].cntmax = 1;
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);
        pull(p);
    }

    void init(const V < ll > &a) {
        n = int(a.size()) - 1;
        t.assign(4 * n + 5, Node());
        if (n) build(1, 1, n, a);
    }

    void add(int p, int l, int r, int x, int y, ll v) {
        if (x <= l && r <= y) { apply_add(p, r - l + 1, v); return; }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (x <= mid) add(p << 1, l, mid, x, y, v);
        if (y > mid) add(p << 1 | 1, mid + 1, r, x, y, v);
        pull(p);
    }

    void chmin(int p, int l, int r, int x, int y, ll v) {
        if (t[p].max1 <= v) return;
        if (x <= l && r <= y && t[p].max2 < v) {
            apply_min(p, v); return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (x <= mid) chmin(p << 1, l, mid, x, y, v);
        if (y > mid) chmin(p << 1 | 1, mid + 1, r, x, y, v);
        pull(p);
    }

    void chmax(int p, int l, int r, int x, int y, ll v) {
        if (t[p].min1 >= v) return;
        if (x <= l && r <= y && t[p].min2 > v) {
            apply_max(p, v); return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (x <= mid) chmax(p << 1, l, mid, x, y, v);
        if (y > mid) chmax(p << 1 | 1, mid + 1, r, x, y, v);
        pull(p);
    }

    ll query(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return t[p].sum;
        push(p, l, r);
        int mid = (l + r) >> 1;
        ll ans = 0;
        if (x <= mid) ans += query(p << 1, l, mid, x, y);
        if (y > mid) ans += query(p << 1 | 1, mid + 1, r, x, y);
        return ans;
    }

    ll query_min(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return t[p].min1;
        push(p, l, r);
        int mid = (l + r) >> 1;
        ll ans = INF;
        if (x <= mid) ans = min(ans, query_min(p << 1, l, mid, x, y));
        if (y > mid) ans = min(ans, query_min(p << 1 | 1, mid + 1, r, x, y));
        return ans;
    }

    ll query_max(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return t[p].max1;
        push(p, l, r);
        int mid = (l + r) >> 1;
        ll ans = -INF;
        if (x <= mid) ans = max(ans, query_max(p << 1, l, mid, x, y));
        if (y > mid) ans = max(ans, query_max(p << 1 | 1, mid + 1, r, x, y));
        return ans;
    }

    void add(int l, int r, ll x) { add(1, 1, n, l, r, x); }
    void chmin(int l, int r, ll x) { chmin(1, 1, n, l, r, x); }
    void chmax(int l, int r, ll x) { chmax(1, 1, n, l, r, x); }
    ll query(int l, int r) { return query(1, 1, n, l, r); }
    ll query_min(int l, int r) { return query_min(1, 1, n, l, r); }
    ll query_max(int l, int r) { return query_max(1, 1, n, l, r); }
};
const ll Beats::INF;
// 用法：Beats st; st.init({0, 3, 1, 4});
// st.add(1, 3, 2); st.chmin(1, 2, 4); st.chmax(2, 3, 5);
// ll sum = st.query(1, 3); // 15
// ll low = st.query_min(1, 3), high = st.query_max(1, 3); // 4, 6
