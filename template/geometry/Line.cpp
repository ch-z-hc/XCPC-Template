#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制 Point.cpp；直线端点不能重合，isLL 要求两线不平行。
// 以下交点、相交判断、投影和距离均 O(1)，无模数。

bool checkPll(P p1, P p2, P q1, P q2) {
    // 判断平行
    db a1 = cross(q1, q2, p1), a2 = - cross(q1, q2, p2);
    return sign(a1 + a2) == 0;
}

P isLL(P p1, P p2, P q1, P q2) {
    // 返回交点
    db a1 = cross(q1, q2, p1), a2 = - cross(q1, q2, p2);
    return (p1 * a2 + p2 * a1) / (a1 + a2);
}

bool intersect(db l1, db r1, db l2, db r2) {
    if (l1 > r1) swap(l1, r1);
    if (l2 > r2) swap(l2, r2);
    return cmp(r1, l2) >= 0 && cmp(r2, l1) >= 0;
}

// 严格相交：不在边界交叉
// 不严格相交：可以在边界交叉

bool isSS(P p1, P p2, P q1, P q2) {
    // 线段是否 不严格 相交
    // 1 : SS
    // 0 : unSS
    return intersect(p1.x, p2.x, q1.x, q2.x) &&
           intersect(p1.y, p2.y, q1.y, q2.y) &&
           crossop(p1, p2, q1) * crossop(p1, p2, q2) <= 0 &&
           crossop(q1, q2, p1) * crossop(q1, q2, p2) <= 0;
}

bool isSS_strict(P p1, P p2, P q1, P q2) {
    // 线段是否 严格 相交
    // 1 : SS
    // 0 : unSS
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) < 0 &&
           crossop(q1, q2, p1) * crossop(q1, q2, p2) < 0;
}

bool isLS(P p1, P p2, P q1, P q2) {
    // 直线 p1p2 是否 和线段 q1q2 不严格 相交
    // 1 : LS
    // 0 : unLS
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) <= 0;
}

bool isLS_strict(P p1, P p2, P q1, P q2) {
    // 直线 p1p2 是否 严格 穿过线段 q1q2
    // 1 : LS
    // 0 : unLS
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) < 0;
}


bool isMiddle(db a, db m, db b) {
    return sign(a - m) == 0 || sign(b - m) == 0 || ((a < m) != (b < m));
}

bool isMiddle(P a, P m, P b) {
    return isMiddle(a.x, m.x, b.x) && isMiddle(a.y, m.y, b.y);
}

bool onSeg(P p1, P p2, P q) {
    // 是否在线段上 不严格
    return crossop(p1, p2, q) == 0 && isMiddle(p1, q, p2);
}

bool onSeg_strict(P p1, P p2, P q) {
    // 是否在线段上 严格
    return crossop(p1, p2, q) == 0 &&
           sign((q - p1).dot(p1 - p2)) *
           sign((q - p2).dot(p1 - p2)) < 0;
}

P proj(P p1, P p2, P q) {
    // 求 q 到 p1 p2 的投影
    P dir = p2 - p1;
    return p1 + (p2 - p1) * (dir.dot(q - p1) / (dir).abs2());
}

P reflect(P p1, P p2, P q) {
    // 求 q 到 p1 p2 的反射点
    return proj(p1, p2, q) * 2 - q;
}

db nearest(P p1, P p2, P q) {
    // 求 q 到 p1 p2（线段）的最近点
    if (p1 == p2) return p1.distTo(q);
    P h = proj(p1, p2, q);
    if (isMiddle(p1, h, p2))
        return q.distTo(h);
    return min(p1.distTo(q), p2.distTo(q));
}

db disSS(P p1, P p2, P q1, P q2) {
    // 求线段 p1p2 和 线段 q1q2 的距离
    if (isSS(p1, p2, q1, q2)) return 0;
    db val1 = min(nearest(p1, p2, q1), nearest(p1, p2, q2));
    db val2 = min(nearest(q1, q2, p1), nearest(q1, q2, p2));
    return min(val1, val2);
}

// 有向直线 p -> p + v，半平面为其左侧；v 归一化，side 为带符号距离。
struct Line {
    P p, v;
    db ang;
    Line(P a, P b) : p(a), v((b - a).unit()), ang(v.alpha()) {
        if (ang < 0) ang += 2 * acosl(-1);
    }
    db side(P x) { return v.det(x - p); }
};
