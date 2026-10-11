#include <bits/stdc++.h>

#define V vector

typedef long double ld;
__extension__ typedef __int128 LiChaoInt;

using namespace std;

// 离散整数横坐标上的最大值 Li Chao Tree；查询横坐标必须位于 [L, R]。
// add_line 为 O(log X)，add_seg 为 O(log^2 X)，query 为 O(log X)。
// 空间 O(X + q)，q 为插入次数。
// 整数参数使用有理数精确比较，同值优先返回较早加入的编号。
// 整数 k、b、y 的绝对值不超过 10^18，横坐标为 int；需 GCC / Clang。
// 浮点参数保留浮点比较；不要混入浮点直线后仍要求整数精确判等。

struct LiChaoTree {
    struct Line {
        ld k = 0, b = 0;
        int id = 0;
        LiChaoInt a = 0, c = 0;
        long long d = 0;

        ld eval(int x) const {
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
        int n = max(0, R - L + 1);
        tr.assign(4 * n + 4, 0);
        line.assign(1, Line{});
    }

    void clear() {
        fill(tr.begin(), tr.end(), 0);
        line.assign(1, Line{});
    }

    template < class T, class U >
    int add_line(T k, U b) {
        int id = new_line(k, b);
        if constexpr (is_integral_v < T > && is_integral_v < U >) {
            line[id].a = k; line[id].c = b; line[id].d = 1;
        }
        if (L <= R) insert(1, L, R, id);
        return id;
    }

    template < class T, class U >
    int add_seg(int x0, T left, int x1, U right) {
        using Y = conditional_t < is_integral_v < T > && is_integral_v < U >,
                                  long long, ld >;
        Y y0 = left, y1 = right;
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }
        ld k = x0 == x1 ? 0 : (ld(y1) - ld(y0)) / (ld(x1) - ld(x0));
        ld b = x0 == x1 ? max(y0, y1) : y0 - k * x0;
        int id = new_line(k, b);
        if constexpr (is_integral_v < Y >) {
            auto &s = line[id];
            s.d = x0 == x1 ? 1 : static_cast<long long> (x1) - x0;
            s.a = x0 == x1 ? 0 : LiChaoInt(y1) - y0;
            s.c = x0 == x1 ? max(y0, y1) : LiChaoInt(y0) * s.d - s.a * x0;
        }
        int ql = max(L, x0), qr = min(R, x1);
        if (ql <= qr)
            modify(1, L, R, ql, qr, id);
        return id;
    }

    int query_id(int x) const {
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

    ld query(int x) const {
        int id = query_id(x);
        return id ? line[id].eval(x) : -numeric_limits < ld >::infinity();
    }

    int new_line(ld k, ld b) {
        int id = int(line.size());
        line.push_back({k, b, id});
        return id;
    }

    bool better(int u, int v, int x) const {
        if (u == 0) return false;
        if (v == 0) return true;
        if (line[u].d && line[v].d) {
            const auto &a = line[u], &b = line[v];
            LiChaoInt left = (a.a * x + a.c) * b.d;
            LiChaoInt right = (b.a * x + b.c) * a.d;
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

// 调用示例：
// LiChaoTree tr(L, R);
// int id1 = tr.add_line(k, b);
// int id2 = tr.add_seg(x0, y0, x1, y1);
// ld ans = tr.query(x);
