#include <bits/stdc++.h>
using namespace std;

// 区间加、区间求和；下标约定为 1-based。
// build 复杂度 O(n)，add 和 query 复杂度 O(log n)，空间 O(n)。
struct SegmentTree {
    int n = 0;
    vector<long long> sum, lazy;

    void init(int n_) {
        n = n_;
        sum.assign(4 * n + 4, 0);
        lazy.assign(4 * n + 4, 0);
    }

    void build(const vector<long long> &a, int p, int l, int r) {
        if (l == r) {
            sum[p] = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(a, p << 1, l, mid);
        build(a, p << 1 | 1, mid + 1, r);
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    void build(const vector<long long> &a) {
        init(int(a.size()) - 1);
        if (n > 0) build(a, 1, 1, n);
    }

    void apply(int p, int l, int r, long long v) {
        sum[p] += v * (r - l + 1);
        lazy[p] += v;
    }

    void push(int p, int l, int r) {
        if (!lazy[p] || l == r) return;
        int mid = (l + r) >> 1;
        apply(p << 1, l, mid, lazy[p]);
        apply(p << 1 | 1, mid + 1, r, lazy[p]);
        lazy[p] = 0;
    }

    void add(int ql, int qr, long long v, int p, int l, int r) {
        if (ql <= l && r <= qr) {
            apply(p, l, r, v);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) add(ql, qr, v, p << 1, l, mid);
        if (mid < qr) add(ql, qr, v, p << 1 | 1, mid + 1, r);
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    void add(int l, int r, long long v) {
        if (n && l <= r) add(l, r, v, 1, 1, n);
    }

    long long query(int ql, int qr, int p, int l, int r) {
        if (ql <= l && r <= qr) return sum[p];
        push(p, l, r);
        int mid = (l + r) >> 1;
        long long ret = 0;
        if (ql <= mid) ret += query(ql, qr, p << 1, l, mid);
        if (mid < qr) ret += query(ql, qr, p << 1 | 1, mid + 1, r);
        return ret;
    }

    long long query(int l, int r) {
        return n && l <= r ? query(l, r, 1, 1, n) : 0;
    }
};

// Usage:
// vector<long long> a(n + 1);
// SegmentTree st;
// st.build(a);
// st.add(l, r, v);
// long long ans = st.query(l, r);
