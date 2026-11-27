/*
区间加、区间和、区间内按值搜索，a[1..n]，和须放入 ll。按前缀和找第 k 项须保证各项非负。
建树 O(n)，单次修改和查询 O(log n)。
first_ge、last_ge 找区间首个、末个 >= x 的位置，kth 找前缀和首次达到 k 的位置。
三种搜索无解均返回 -1，kth 还要求 k > 0。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

struct SegTree {
    int n = 0;
    V < ll > sum, mx, lazy;

    void pull(int p) {
        sum[p] = sum[p * 2] + sum[p * 2 + 1];
        mx[p] = max(mx[p * 2], mx[p * 2 + 1]);
    }

    void apply(int p, int l, int r, ll v) {
        sum[p] += v * (r - l + 1);
        mx[p] += v;
        lazy[p] += v;
    }

    void push(int p, int l, int r) {
        if (!lazy[p] || l == r) return;
        int mid = (l + r) / 2;
        apply(p * 2, l, mid, lazy[p]);
        apply(p * 2 + 1, mid + 1, r, lazy[p]);
        lazy[p] = 0;
    }

    void build(int p, int l, int r, const V < ll > &a) {
        if (l == r) {
            sum[p] = mx[p] = a[l];
            return;
        }
        int mid = (l + r) / 2;
        build(p * 2, l, mid, a);
        build(p * 2 + 1, mid + 1, r, a);
        pull(p);
    }

    void init(const V < ll > &a) {
        if (&a == &sum || &a == &mx || &a == &lazy) {
            V < ll > b = a;
            init(b);
            return;
        }
        n = int(a.size()) - 1;
        sum.assign(n * 4 + 5, 0); mx.assign(n * 4 + 5, 0);
        lazy.assign(n * 4 + 5, 0);
        if (n > 0) build(1, 1, n, a);
    }

    void modify(int p, int l, int r, int x, int y, ll v) {
        if (x <= l && r <= y) {
            apply(p, l, r, v);
            return;
        }
        push(p, l, r);
        int mid = (l + r) / 2;
        if (x <= mid) modify(p * 2, l, mid, x, y, v);
        if (y > mid) modify(p * 2 + 1, mid + 1, r, x, y, v);
        pull(p);
    }

    void modify(int l, int r, ll v) {
        if (n && l <= r) modify(1, 1, n, l, r, v);
    }

    ll query(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return sum[p];
        push(p, l, r);
        int mid = (l + r) / 2;
        ll ans = 0;
        if (x <= mid) ans += query(p * 2, l, mid, x, y);
        if (y > mid) ans += query(p * 2 + 1, mid + 1, r, x, y);
        return ans;
    }

    ll query(int l, int r) { return n && l <= r ? query(1, 1, n, l, r) : 0; }

    int first_ge(int p, int l, int r, int x, int y, ll v) {
        if (r < x || y < l || mx[p] < v) return -1;
        if (l == r) return l;
        push(p, l, r);
        int mid = (l + r) / 2;
        int ans = first_ge(p * 2, l, mid, x, y, v);
        return ans != -1 ? ans : first_ge(p * 2 + 1, mid + 1, r, x, y, v);
    }

    int first_ge(int l, int r, ll v) {
        return n && l <= r ? first_ge(1, 1, n, l, r, v) : -1;
    }

    int last_ge(int p, int l, int r, int x, int y, ll v) {
        if (r < x || y < l || mx[p] < v) return -1;
        if (l == r) return l;
        push(p, l, r);
        int mid = (l + r) / 2;
        int ans = last_ge(p * 2 + 1, mid + 1, r, x, y, v);
        return ans != -1 ? ans : last_ge(p * 2, l, mid, x, y, v);
    }

    int last_ge(int l, int r, ll v) {
        return n && l <= r ? last_ge(1, 1, n, l, r, v) : -1;
    }

    int kth(ll k) {
        if (!n || k <= 0 || k > sum[1]) return -1;
        int p = 1, l = 1, r = n;
        while (l < r) {
            push(p, l, r);
            int mid = (l + r) / 2;
            if (k <= sum[p * 2]) {
                p *= 2; r = mid;
            } else {
                k -= sum[p * 2];
                p = p * 2 + 1; l = mid + 1;
            }
        }
        return l;
    }
};

/*
SegTree st; st.init({0, 2, 1, 5}); st.modify(1, 2, 1);
ll sum = st.query(1, 3);
int l = st.first_ge(1, 3, 3), r = st.last_ge(1, 3, 3);
int p = st.kth(4);
*/
