/*
区间加、取上限 chmin、取下限 chmax，查询区间最小值和最大值。
modify(l, r, d, lo, hi) 将各项 x 改为 clamp(x + d, lo, hi)，lo <= hi。
函数单调，截断的结果只需看区间两端的最值。下传时先加偏移，再截到父节点的最值之间。
建树 O(n)，每次修改、查询 O(log n)。a[1..n]，区间采用 1-based 闭区间。
各节点累计偏移须放入 ll。x + d 超出 ll 时按截断边界取值。
空区间查询返回 {LLONG_MAX, LLONG_MIN}。
*/

#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
typedef long long ll;
using namespace std;

struct ClampSeg {
    struct Node {
        ll mn = 0, mx = 0, add = 0;
    };
    int n = 0;
    V < Node > t;

    void pull(int p) {
        t[p].mn = min(t[p * 2].mn, t[p * 2 + 1].mn);
        t[p].mx = max(t[p * 2].mx, t[p * 2 + 1].mx);
    }

    ll shift(ll x, ll d, ll lo, ll hi) {
        if (d > 0 && x > LLONG_MAX - d) return hi;
        if (d < 0 && x < LLONG_MIN - d) return lo;
        return clamp(x + d, lo, hi);
    }

    void apply(int p, ll d, ll lo, ll hi) {
        auto &u = t[p];
        if (lo == hi) u.mn = u.mx = lo;
        else {
            u.mn = shift(u.mn, d, lo, hi);
            u.mx = shift(u.mx, d, lo, hi);
        }
        u.add = u.mn == u.mx ? 0 : u.add + d;
    }

    void push(int p) {
        auto &u = t[p];
        apply(p * 2, u.add, u.mn, u.mx);
        apply(p * 2 + 1, u.add, u.mn, u.mx);
        u.add = 0;
    }

    void build(int p, int l, int r, const V < ll > &a) {
        if (l == r) {
            t[p].mn = t[p].mx = a[l];
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

    void modify(int p, int l, int r, int x, int y, ll d, ll lo, ll hi) {
        if (x <= l && r <= y) {
            apply(p, d, lo, hi);
            return;
        }
        push(p);
        int mid = l + (r - l) / 2;
        if (x <= mid) modify(p * 2, l, mid, x, y, d, lo, hi);
        if (y > mid) modify(p * 2 + 1, mid + 1, r, x, y, d, lo, hi);
        pull(p);
    }

    pair < ll, ll > query(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return {t[p].mn, t[p].mx};
        push(p);
        int mid = l + (r - l) / 2;
        if (y <= mid) return query(p * 2, l, mid, x, y);
        if (x > mid) return query(p * 2 + 1, mid + 1, r, x, y);
        auto a = query(p * 2, l, mid, x, y);
        auto b = query(p * 2 + 1, mid + 1, r, x, y);
        return {min(a.fi, b.fi), max(a.se, b.se)};
    }

    void modify(int l, int r, ll d, ll lo, ll hi) {
        if (n && l <= r) modify(1, 1, n, l, r, d, lo, hi);
    }
    void add(int l, int r, ll d) { modify(l, r, d, LLONG_MIN, LLONG_MAX); }
    void chmin(int l, int r, ll hi) { modify(l, r, 0, LLONG_MIN, hi); }
    void chmax(int l, int r, ll lo) { modify(l, r, 0, lo, LLONG_MAX); }

    pair < ll, ll > query(int l, int r) {
        if (!n || l > r) return {LLONG_MAX, LLONG_MIN};
        return query(1, 1, n, l, r);
    }
};

/*
ClampSeg st; st.init({0, 3, 1, 4});
st.add(1, 3, 2);
st.chmin(1, 2, 4); st.chmax(2, 3, 5);
st.modify(1, 3, -2, 3, 8);
auto [low, high] = st.query(1, 3);
*/
