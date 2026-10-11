#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 区间加、区间求和；下标为 1-based，区间为闭区间 [l, r]。
// 初始化 O(n)，modify 和 query 为 O(log n)，空间 O(n)。
// 要求 a 保留下标 0 的占位，所有计算不溢出 ll；init 可重新构建。
// 空区间修改无效，查询返回 0；不使用模数。
struct SegTree {
    int n = 0;
    V < ll > sum, lazy;

    void pull(int p) {
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    void apply(int p, int l, int r, ll v) {
        sum[p] += v * (r - l + 1);
        lazy[p] += v;
    }

    void push(int p, int l, int r) {
        if (! lazy[p] || l == r) return;
        int mid = (l + r) >> 1;
        apply(p << 1, l, mid, lazy[p]);
        apply(p << 1 | 1, mid + 1, r, lazy[p]);
        lazy[p] = 0;
    }

    void build(int p, int l, int r, const V < ll > &a) {
        if (l == r) { sum[p] = a[l]; return; }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);
        pull(p);
    }

    void init(const V < ll > &a) {
        n = int(a.size()) - 1;
        sum.assign(4 * n + 5, 0);
        lazy.assign(4 * n + 5, 0);
        if (n > 0) build(1, 1, n, a);
    }

    void modify(int p, int l, int r, int x, int y, ll v) {
        if (x <= l && r <= y) { apply(p, l, r, v); return; }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (x <= mid) modify(p << 1, l, mid, x, y, v);
        if (y > mid) modify(p << 1 | 1, mid + 1, r, x, y, v);
        pull(p);
    }

    void modify(int l, int r, ll v) {
        if (n && l <= r) modify(1, 1, n, l, r, v);
    }

    ll query(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return sum[p];
        push(p, l, r);
        int mid = (l + r) >> 1;
        ll ans = 0;
        if (x <= mid) ans += query(p << 1, l, mid, x, y);
        if (y > mid) ans += query(p << 1 | 1, mid + 1, r, x, y);
        return ans;
    }

    ll query(int l, int r) {
        return n && l <= r ? query(1, 1, n, l, r) : 0;
    }
};

// 调用示例：
// V < ll > a(n + 1);
// SegTree st;
// st.init(a);
// st.modify(l, r, v);
// ll ans = st.query(l, r);
