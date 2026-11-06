#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制本章的“点与向量”“直线与线段”；坐标有限、半径非负。
// 圆相关操作均 O(1)。
// 浮点误差为 EPS；公切线要求正半径，重合圆交点 / 切线约定返回空。

struct Circle {
    P o;
    db r = 0;
    bool contains(P p) const { return p.distTo(o) <= r + EPS; }
};

Circle diameter_circle(P a, P b) {
    return {(a + b) / 2, a.distTo(b) / 2};
}

// 三点共线返回 false，否则将外接圆写入 ans 并返回 true。
bool circumcircle(P a, P b, P c, Circle &ans) {
    P u = b - a, v = c - a;
    db d = 2 * u.det(v);
    if (d == 0) return false;
    P o = a + (u.rot90() * v.abs2() - v.rot90() * u.abs2()) / d;
    ans = {o, o.distTo(a)};
    return true;
}

// 圆与无限直线的交点，返回 0 / 1 / 2 个点。
V < P > isCL(Circle c, P a, P b) {
    P v = (b - a).unit(), p = a + v * (c.o - a).dot(v);
    db d = p.distTo(c.o);
    if (d > c.r + EPS) return {};
    db h = sqrtl(max(0.L, (c.r - d) * (c.r + d)));
    if (h <= EPS) return {p};
    return {p - v * h, p + v * h};
}

// 两圆交点，返回 0 / 1 / 2 个点；重合圆有无穷多交点，约定返回空。
V < P > isCC(Circle a, Circle b) {
    P v = b.o - a.o;
    db d = v.abs();
    if (d <= EPS) {
        if (a.r == 0 && b.r == 0) return {a.o};
        return {};
    }
    if (d > a.r + b.r + EPS || d < abs(a.r - b.r) - EPS) return {};
    db x = (a.r * a.r - b.r * b.r + d * d) / (2 * d);
    db h = sqrtl(max(0.L, a.r * a.r - x * x));
    P p = a.o + v * (x / d), w = v.rot90() * (h / d);
    if (h <= EPS) return {p};
    return {p - w, p + w};
}

// 从点 p 向圆作切线，返回切点；圆内为空，圆上为一个点，圆外为两个。
V < P > tangent_points(Circle c, P p) {
    P v = p - c.o;
    db d = v.abs(), r = c.r;
    if (d < r - EPS) return {};
    if (abs(d - r) <= EPS) return {p};
    P q = c.o + v * (r * r / (d * d));
    P w = v.rot90() * (r * sqrtl(max(0.L, d * d - r * r)) / (d * d));
    if (r <= EPS) return {c.o};
    return {q - w, q + w};
}

// 两个正半径圆的公切线，包含内外公切线；重合圆约定返回空。
V < Line > common_tangents(Circle a, Circle b) {
    P v = b.o - a.o;
    db d = v.abs();
    V < Line > ans;
    if (d <= EPS) return ans;
    v = v / d;
    for (int s : {-1, 1}) {
        db c = (a.r - s * b.r) / d;
        if (abs(c) > 1 + EPS) continue;
        c = clamp(c, -1.L, 1.L);
        db h = sqrtl(max(0.L, 1 - c * c));
        for (int side : {-1, 1}) {
            P normal = v * c + v.rot90() * (side * h);
            P p = a.o + normal * a.r;
            ans.emplace_back(p, p + normal.rot90());
            if (h <= EPS) break;
        }
    }
    return ans;
}

db circle_area(Circle a, Circle b) {
    const db pi = acosl(-1);
    db d = a.o.distTo(b.o);
    if (d >= a.r + b.r) return 0;
    if (d <= abs(a.r - b.r)) return pi * min(a.r, b.r) * min(a.r, b.r);
    db x = acosl(clamp((d * d + a.r * a.r - b.r * b.r) / (2 * d * a.r),
                      -1.L, 1.L));
    db y = acosl(clamp((d * d + b.r * b.r - a.r * a.r) / (2 * d * b.r),
                      -1.L, 1.L));
    return a.r * a.r * (x - sinl(2 * x) / 2) + b.r * b.r * (y - sinl(2 * y) / 2);
}
