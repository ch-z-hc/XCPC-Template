#include <bits/stdc++.h>

#define V vector

typedef long double ld;
typedef long long ll;

using namespace std;

/*
在整数横坐标 [L, R] 维护直线、线段的最大值。X = R - L + 1，建树和清空 O(X)。
插直线、查询 O(log X)，插线段 O(log² X)。密集建树约需 4X 个 int，X <= (INT_MAX - 4) / 4。
整数端点差、a*x+c 及与另一分母的乘积须放入 ll。小数斜率、截距或纵坐标用浮点接口。
*/

struct LiChao {
    struct Line {
        ld k = 0, b = 0;
        int id = 0;
        ll a = 0, c = 0, d = 0;

        ld eval(int x) {
            return d ? ld(a * x + c) / d : k * x + b;
        }
    };

    int L = 1, R = 0;
    V < int > tr;
    V < Line > line;

    LiChao(int r = 0) { init(1, r); }
    LiChao(int l, int r) { init(l, r); }

    void init(int r) { init(1, r); }

    void init(int l, int r) {
        L = l;
        R = r;
        ll n = max(0ll, 1ll * R - L + 1);
        assert(n <= (INT_MAX - 4) / 4);
        tr.assign(n * 4 + 4, 0);
        line.assign(1, Line{});
    }

    void clear() {
        fill(tr.begin(), tr.end(), 0);
        line.assign(1, Line{});
    }

    int add_line(ll k, ll b) {
        int id = new_line(k, b);
        line[id].a = k;
        line[id].c = b;
        line[id].d = 1;
        if (L <= R) insert(1, L, R, id);
        return id;
    }

    int add_seg(int x0, ll y0, int x1, ll y1) {
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }
        ld k = x0 == x1 ? 0 : (ld(y1) - ld(y0)) / (ld(x1) - ld(x0));
        ld b = x0 == x1 ? max(y0, y1) : y0 - k * x0;
        int id = new_line(k, b);
        auto &s = line[id];
        s.d = x0 == x1 ? 1 : 1ll * x1 - x0;
        s.a = x0 == x1 ? 0 : y1 - y0;
        s.c = x0 == x1 ? max(y0, y1) : y0 * s.d - s.a * x0;
        int ql = max(L, x0), qr = min(R, x1);
        if (ql <= qr)
            modify(1, L, R, ql, qr, id);
        return id;
    }

    int add_line_real(ld k, ld b) {
        int id = new_line(k, b);
        if (L <= R) insert(1, L, R, id);
        return id;
    }

    int add_seg_real(int x0, ld y0, int x1, ld y1) {
        if (x0 > x1) { swap(x0, x1); swap(y0, y1); }
        ld k = x0 == x1 ? 0 : (y1 - y0) / (ld(x1) - ld(x0));
        ld b = x0 == x1 ? max(y0, y1) : y0 - k * x0;
        int id = new_line(k, b);
        int ql = max(L, x0), qr = min(R, x1);
        if (ql <= qr) modify(1, L, R, ql, qr, id);
        return id;
    }

    int query_id(int x) {
        int p = 1, l = L, r = R, ans = 0;
        while (l <= r) {
            if (better(tr[p], ans, x)) ans = tr[p];
            if (l == r) break;
            int mid = l + (r - l) / 2;
            if (x <= mid) {
                p *= 2;
                r = mid;
            } else {
                p = p * 2 + 1;
                l = mid + 1;
            }
        }
        return ans;
    }

    ld query(int x) {
        int id = query_id(x);
        return id ? line[id].eval(x) : -numeric_limits < ld >::infinity();
    }

    bool query_int(int x, ll &ans) {
        int id = query_id(x);
        if (! id || line[id].d != 1) return false;
        ans = line[id].a * x + line[id].c;
        return true;
    }

    int new_line(ld k, ld b) {
        int id = int(line.size());
        line.push_back({k, b, id});
        return id;
    }

    bool better(int u, int v, int x) {
        if (u == 0) return false;
        if (v == 0) return true;
        if (line[u].d && line[v].d) {
            auto &a = line[u], &b = line[v];
            ll left = (a.a * x + a.c) * b.d;
            ll right = (b.a * x + b.c) * a.d;
            return left != right ? left > right : u < v;
        }
        ld a = line[u].eval(x);
        ld b = line[v].eval(x);
        if (a == b) return u < v;
        return a > b;
    }

    void insert(int p, int l, int r, int id) {
        if (tr[p] == 0) {
            tr[p] = id;
            return;
        }
        int mid = l + (r - l) / 2;
        if (better(id, tr[p], mid)) swap(tr[p], id);
        if (l == r) return;
        if (better(id, tr[p], l))
            insert(p * 2, l, mid, id);
        else if (better(id, tr[p], r))
            insert(p * 2 + 1, mid + 1, r, id);
    }

    void modify(int p, int l, int r, int ql, int qr, int id) {
        if (ql <= l && r <= qr) {
            insert(p, l, r, id);
            return;
        }
        int mid = l + (r - l) / 2;
        if (ql <= mid)
            modify(p * 2, l, mid, ql, qr, id);
        if (mid < qr)
            modify(p * 2 + 1, mid + 1, r, ql, qr, id);
    }
};

/*
LiChao tr(1, 10);                   // tr.init(10) 或 tr.init(1, 10) 重设值域
int first = tr.add_line(3, -2);     // y = 3x - 2，插入返回编号
ll exact; if (tr.query_int(2, exact)) cout << exact; // 4，只插整数直线时使用
tr.add_seg(1, 2, 5, 9);             // 两端点 (1, 2)、(5, 9)，区间外无效
tr.add_line_real(0.5L, 1.2L); tr.add_seg_real(2, 1.5L, 6, 3.5L);
int id = tr.query_id(2);             // 同值取最早编号，未覆盖为 0
ld y = tr.query(2);                  // 近似值，未覆盖为负无穷
tr.clear();                          // 保留值域，清空所有线
*/
