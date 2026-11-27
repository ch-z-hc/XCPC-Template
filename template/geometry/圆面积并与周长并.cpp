/*
先复制「点与向量」「直线与线段」「圆」。求多圆并的面积和周长，含洞的边界，O(n² log n)。
坐标有限、半径非负，重合圆只计一次；极近退化时受浮点精度限制。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

#define db long double

pair < db, db > circle_union(const V < Circle > &cs) {
    const db pi = acosl(-1), tau = 2 * pi;
    int n = cs.size();
    db area = 0, perimeter = 0;
    if (!n) return {0, 0};
    P origin = cs[0].o;
    for (int i = 0; i < n; i ++) {
        Circle a = cs[i];
        if (a.r == 0) continue;
        V < pair < db, db > > covered;
        bool hidden = 0;
        for (int j = 0; j < n; j ++) {
            if (i == j || cs[j].r == 0) continue;
            Circle b = cs[j];
            P v = b.o - a.o;
            db d = v.abs();
            if (d == 0 && a.r == b.r) {
                if (j < i) {
                    hidden = 1;
                    break;
                }
                continue;
            }
            if (d + a.r <= b.r) {
                hidden = 1;
                break;
            }
            if (d >= a.r + b.r || d + b.r <= a.r) continue;
            db mid = atan2l(v.y, v.x);
            db angle =
                acosl(clamp((d * d + a.r * a.r - b.r * b.r) / (2 * d * a.r), -1.L, 1.L));
            db l = mid - angle, r = mid + angle;
            while (l < 0) {
                l += tau; r += tau;
            }
            while (l >= tau) {
                l -= tau; r -= tau;
            }
            if (r <= tau)
                covered.push_back({l, r});
            else {
                covered.push_back({l, tau});
                covered.push_back({0, r - tau});
            }
        }
        if (hidden) continue;
        sort(covered.begin(), covered.end());
        P o = a.o - origin;
        auto add_arc = [&](db l, db r) {
            if (l >= r) return;
            perimeter += a.r * (r - l);
            area += (a.r * a.r * (r - l) + a.r * o.x * (sinl(r) - sinl(l)) +
                     a.r * o.y * (cosl(l) - cosl(r))) /
                    2;
        };
        db last = 0;
        for (auto [l, r] : covered) {
            add_arc(last, l);
            last = max(last, r);
        }
        add_arc(last, tau);
    }
    return {max(0.L, area), perimeter};
}

// auto [area, perimeter] = circle_union(cs);
