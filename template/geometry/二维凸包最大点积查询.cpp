#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 独立整点片段，使用 HullQuery::P，不依赖浮点 P / EPS。
// build 允许乱序、重复、共线和空点集；O(n log n) 时间、O(n) 空间。
// query(a, b, ans) 求 max(a * x + b * y)，O(log n)；空集返回 false。
// a、b 可正可负或为 0；返回 false 时不可使用 ans。
// contains(p) 为 O(log n)，返回 0 外部 / 1 边界 / 2 内部。
// h 是严格逆时针凸包，不重复首点；点、线退化只保留 1 / 2 个端点。
// 所有点、包含查询坐标及 a、b 的绝对值 <= 1e9，可保证 ll 不溢出。
// 扩大范围时须保证坐标差、叉积中间量和点积全部放入 ll。

struct HullQuery {
    struct P {
        ll x, y;
        bool operator <(P p) const {
            return x != p.x ? x < p.x : y < p.y;
        }
        bool operator ==(P p) const { return x == p.x && y == p.y; }
    };
    V < P > lo, hi, h;

    ll turn(P a, P b, P c) {
        return (b.x - a.x) * (c.y - a.y) -
               (b.y - a.y) * (c.x - a.x);
    }

    void build(V < P > p) {
        lo.clear(); hi.clear(); h.clear();
        sort(p.begin(), p.end());
        p.erase(unique(p.begin(), p.end()), p.end());
        for (P q : p) {
            while (lo.size() > 1 &&
                   turn(lo[lo.size() - 2], lo.back(), q) <= 0)
                lo.pop_back();
            lo.push_back(q);
            while (hi.size() > 1 &&
                   turn(hi[hi.size() - 2], hi.back(), q) >= 0)
                hi.pop_back();
            hi.push_back(q);
        }
        h = lo;
        for (int i = (int)hi.size() - 2; i > 0; -- i)
            h.push_back(hi[i]);
    }

    bool query(ll a, ll b, ll &ans) {
        if (h.empty()) return false;
        auto &p = b >= 0 ? hi : lo;
        auto dot = [&](int i) { return a * p[i].x + b * p[i].y; };
        if (b == 0) {
            ans = max(dot(0), dot((int)p.size() - 1));
            return true;
        }
        // 选取对应的上 / 下凸链后，点积沿链先增后减。
        int l = 0, r = (int)p.size() - 1;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (dot(mid) < dot(mid + 1)) l = mid + 1;
            else r = mid;
        }
        ans = dot(l);
        return true;
    }

    bool on_seg(P a, P b, P p) {
        return turn(a, b, p) == 0 &&
               min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
               min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
    }

    int contains(P p) {
        int n = h.size();
        if (! n) return 0;
        if (n == 1) return h[0] == p;
        if (n == 2) return on_seg(h[0], h[1], p);
        ll left = turn(h[0], h[1], p);
        ll right = turn(h[0], h[n - 1], p);
        if (left < 0 || right > 0) return 0;
        if (! left) return on_seg(h[0], h[1], p);
        if (! right) return on_seg(h[0], h[n - 1], p);
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
// if (hull.query(1, 2, ans)) { /* ans = 6 */ }
// int state = hull.contains({0, 1}); // 1，边界。
