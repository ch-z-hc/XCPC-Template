#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 独立整点凸包，使用 HullQuery::P；build 支持空集、重复和共线点。
// 建凸包 O(n log n)，query / contains 为 O(log n)，空间 O(n)。
// query(a, b, ans) 求 max(a * x + b * y)；方向任意，空集返回 0，ans 不可用。
// contains 返回 0 外部 / 1 边界 / 2 内部。
// h 为 0-based 严格逆时针凸包，不重复首点；退化为点 / 线时保留 1 / 2 点。
// 点坐标、包含查询坐标及 a、b 的绝对值 <= 1e9，保证 ll 不溢出。
// 扩大范围须保证坐标差、叉积中间量和点积均放入 ll。

struct HullQuery {
    struct P {
        ll x, y;
        P operator -(P p) { return {x - p.x, y - p.y}; }
        ll dot(P p) { return x * p.x + y * p.y; }
        ll det(P p) { return x * p.y - y * p.x; }
        bool operator < (P p) const {
            return x != p.x ? x < p.x : y < p.y;
        }
        bool operator == (P p) const { return x == p.x && y == p.y; }
    };
    V < P > lo, hi, h;

    ll turn(P a, P b, P c) { return (b - a).det(c - a); }

    void build(V < P > p) {
        lo.clear(); hi.clear(); h.clear();
        sort(p.begin(), p.end());
        p.erase(unique(p.begin(), p.end()), p.end());
        for (P q : p) {
            while (lo.size() > 1 &&
                   turn(lo[lo.size() - 2], lo.back(), q) <= 0) lo.pop_back();
            while (hi.size() > 1 &&
                   turn(hi[hi.size() - 2], hi.back(), q) >= 0) hi.pop_back();
            lo.push_back(q); hi.push_back(q);
        }
        h = lo;
        for (int i = (int)hi.size() - 2; i > 0; -- i) h.push_back(hi[i]);
    }

    bool query(ll a, ll b, ll &ans) {
        if (h.empty()) return 0;
        auto &p = b >= 0 ? hi : lo;
        P q{a, b};
        if (! b) {
            ans = max(p.front().dot(q), p.back().dot(q));
            return 1;
        }
        // 对应凸链上的点积先增后减；水平查询单独取左右端点。
        int l = 0, r = (int)p.size() - 1;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (p[mid].dot(q) < p[mid + 1].dot(q)) l = mid + 1;
            else r = mid;
        }
        ans = p[l].dot(q);
        return 1;
    }

    bool on_seg(P a, P b, P p) {
        return ! turn(a, b, p) &&
               min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
               min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
    }

    int contains(P p) {
        int n = h.size();
        if (! n) return 0;
        if (n == 1) return h[0] == p;
        if (n == 2) return on_seg(h[0], h[1], p);
        ll d1 = turn(h[0], h[1], p), d2 = turn(h[0], h[n - 1], p);
        if (d1 < 0 || d2 > 0) return 0;
        if (! d1) return on_seg(h[0], h[1], p);
        if (! d2) return on_seg(h[0], h[n - 1], p);
        int l = 1, r = n - 1;
        while (r - l > 1) {
            int mid = (l + r) >> 1;
            if (turn(h[0], h[mid], p) >= 0) l = mid;
            else r = mid;
        }
        ll d = turn(h[l], h[r], p);
        return d < 0 ? 0 : d == 0 ? 1 : 2;
    }
};

// HullQuery hull; hull.build({{0, 0}, {2, 0}, {0, 3}}); ll ans;
// if (hull.query(1, 2, ans)) cout << ans << '\n'; // 6
// hull.contains({0, 1}); // 1，边界。
