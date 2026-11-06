#include <bits/stdc++.h>
#include <cassert>

#define V vector

typedef long double ld;
typedef long long ll;

using namespace std;

// 密集整数区间上的最大值 Li Chao Tree；查询横坐标必须位于 [L, R]。
// add_line 为 O(log X)，add_seg 为 O(log^2 X)，query 为 O(log X)。
// X = R - L + 1，按整段坐标开约 4X 个 int。
// 空间 O(X + q)，q 为插入次数；X <= (INT_MAX - 4) / 4 且须能存下数组。
// add_line / add_seg 接收整数，使用有理数精确比较，同值取较早加入的编号。
// ll 中间量不能溢出：端点差、a * x + c 及其与另一条线的 d 的乘积。
// 线段端点在树外时也须满足此限制。
// 充分范围：所有横坐标在 [0, 40000]，|k|、|b|、|y| <= 10^9。
// 浮点参数用 add_line_real / add_seg_real；混用后不保证整数精确判等。
// query 返回浮点近似值；整数 DP 仅插入 add_line，并用 query_int 精确取值。

struct LiChaoTree {
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

    LiChaoTree(int r = 0) { init(1, r); }
    LiChaoTree(int l, int r) { init(l, r); }

    void init(int r) { init(1, r); }

    void init(int l, int r) {
        L = l;
        R = r;
        long long n = max(0ll, 1ll * R - L + 1);
        assert(n <= (INT_MAX - 4) / 4);
        tr.assign(4 * n + 4, 0);
        line.assign(1, Line{});
    }

    void clear() {
        fill(tr.begin(), tr.end(), 0);
        line.assign(1, Line{});
    }

    int add_line(long long k, long long b) {
        int id = new_line(k, b);
        line[id].a = k; line[id].c = b; line[id].d = 1;
        if (L <= R) insert(1, L, R, id);
        return id;
    }

    int add_seg(int x0, long long y0, int x1, long long y1) {
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

    // 只配合 add_line 使用；空集或命中非整数直线返回 false，ans 不可用。
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

// 用法（整数直线）：LiChaoTree tr(1, 10000); long long k = 3, b = -2;
// tr.add_line(k, b); ll ans;
// if (tr.query_int(2, ans)) cout << ans << '\n';
// 整数端点线段：tr.add_seg(1, 2, 5, 7); int id = tr.query_id(3);
// 线段插值可能为分数，近似值用 ld y = tr.query(3)，不能套 query_int。
// 浮点直线另建树：LiChaoTree real(1, 10000);
// real.add_line_real(0.5L, 0.0L); ld y = real.query(2); // y = 1。
// add_line 接收整数；浮点斜率误传入时会被截断。
