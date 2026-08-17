#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;
typedef long double ld;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 

using namespace std;

struct seg {
    ld k, b;
    int id;
};

struct LiChaoTree {
    int cnt, len;
    V < int > t;
    V < seg > a;

    LiChaoTree() {}

    LiChaoTree(int _len) {
        init(_len);
    }

    void init(int _len) {
        len = _len;
        cnt = 0;
        t.assign(len * 4 + 5, 0);
        a.assign(1, {0, 0, 0});
    }

    double calc(int id, int x) {
        if (id == 0) return -1e10;
        return a[id].k * x + a[id].b;
    }

    bool cmp(int x, int y, int p) {
        const double eps = 1e-9;
        if (fabs(calc(x, p) - calc(y, p)) < eps)
            return x < y;
        return calc(x, p) > calc(y, p);
    }

    void query(int p, int l, int r, int k, int & ans) {
        if (cmp(t[p], ans, k)) ans = t[p];
        if (l == r) return;
        int mid = (l + r) / 2;
        if (k <= mid) query(p * 2, l, mid, k, ans);
        else query(p * 2 + 1, mid + 1, r, k, ans);
    }

    void upd(int p, int l, int r, int id) {
        if (t[p] == 0) {
            t[p] = id;
            return;
        }
        int mid = (l + r) / 2;
        if (cmp(id, t[p], mid))
            swap(t[p], id);
        if (l == r) return;
        if (cmp(id, t[p], l))
            upd(p * 2, l, mid, id);
        else if (cmp(id, t[p], r))
            upd(p * 2 + 1, mid + 1, r, id);
    }

    void modify(int p, int l, int r, int s, int e, int id) {
        if (l >= s && r <= e) {
            upd(p, l, r, id);
            return;
        }
        int mid = (l + r) / 2;
        if (s <= mid) modify(p * 2, l, mid, s, e, id);
        if (e > mid) modify(p * 2 + 1, mid + 1, r, s, e, id);
    }

    int insert(int x0, int y0, int x1, int y1) {
        if (x0 > x1) swap(x0, x1), swap(y0, y1);
        ++ cnt;
        a.push_back({0, 0, cnt});
        if (x0 == x1) a[cnt].k = 0, a[cnt].b = max(y0, y1);
        else {
            a[cnt].k = 1.0 * (y1 - y0) / (x1 - x0);
            a[cnt].b = y0 - a[cnt].k * x0;
        }
        modify(1, 1, len, x0, x1, cnt);
        return cnt;
    }

    int ask(int k) {
        int lst = 0;
        query(1, 1, len, k, lst);
        return lst;
    }

    int addline(ld k, ld b) {
        ++ cnt;
        a.push_back({k, b, cnt});
        modify(1, 1, len, 1, len, cnt);
        return cnt;
    }

    void clear() {
        cnt = 0;
        fill(t.begin(), t.end(), 0);
        a.assign(1, {0, 0, 0});
    }

    // ask(x) 返回最优直线编号；value(x) 返回对应函数值。
    long double value(int x) {
        int id = ask(x);
        return id ? calc(id, x) : -1e10L;
    }
};
