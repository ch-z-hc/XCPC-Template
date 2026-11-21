#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
对 a[1..n] 做区间加、查区间和，区间含两端。空区间的和为 0，累加用 ll。
first_ge、last_ge 找 [l, r] 内第一个、最后一个值 >= v 的位置，找不到返回 -1。
kth(k) 找前缀和首次达到 k 的位置。所有元素须非负，k 越界返回 -1。
建树 O(n)，单次修改或查询 O(log n)，空间 O(n)。
*/
struct SegTree {
    int n = 0;
    V < ll > sum, mx, lazy;

    void pull(int p) {
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
        mx[p] = max(mx[p << 1], mx[p << 1 | 1]);
    }

    void apply(int p, int l, int r, ll v) {
        sum[p] += v * (r - l + 1);
        mx[p] += v;
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
        if (l == r) { sum[p] = mx[p] = a[l]; return; }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);
        pull(p);
    }

    void init(const V < ll > &a) {
        if (&a == &sum || &a == &mx || &a == &lazy) {
            V < ll > b = a;
            init(b); return;
        }
        n = int(a.size()) - 1;
        sum.assign(4 * n + 5, 0);
        mx.assign(4 * n + 5, 0);
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

    int first_ge(int p, int l, int r, int x, int y, ll v) {
        if (r < x || y < l || mx[p] < v) return -1;
        if (l == r) return l;
        push(p, l, r);
        int mid = (l + r) >> 1;
        int ans = first_ge(p << 1, l, mid, x, y, v);
        return ans != -1 ? ans : first_ge(p << 1 | 1, mid + 1, r, x, y, v);
    }

    int first_ge(int l, int r, ll v) {
        return n && l <= r ? first_ge(1, 1, n, l, r, v) : -1;
    }

    int last_ge(int p, int l, int r, int x, int y, ll v) {
        if (r < x || y < l || mx[p] < v) return -1;
        if (l == r) return l;
        push(p, l, r);
        int mid = (l + r) >> 1;
        int ans = last_ge(p << 1 | 1, mid + 1, r, x, y, v);
        return ans != -1 ? ans : last_ge(p << 1, l, mid, x, y, v);
    }

    int last_ge(int l, int r, ll v) {
        return n && l <= r ? last_ge(1, 1, n, l, r, v) : -1;
    }

    int kth(ll k) {
        if (! n || k <= 0 || k > sum[1]) return -1;
        int p = 1, l = 1, r = n;
        while (l < r) {
            push(p, l, r);
            int mid = (l + r) >> 1;
            if (k <= sum[p << 1]) p <<= 1, r = mid;
            else k -= sum[p << 1], p = p << 1 | 1, l = mid + 1;
        }
        return l;
    }
};

/*
SegTree st; st.init(a); st.modify(l, r, v); ll ans = st.query(l, r);
int first = st.first_ge(l, r, v), last = st.last_ge(l, r, v), id = st.kth(k);
*/
