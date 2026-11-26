/*
先复制「点与向量」。直线和线段的相交、投影、反射、距离均为 O(1)。
定义直线的两点须不同，两直线求交点前须排除平行。
isSS 判断两线段相交，isLS 判断直线 p1p2 与线段 q1q2 相交。默认含边界，_strict 要求严格相交。
onSeg 含端点，onSeg_strict 排除端点。isMiddle(a, m, b) 的点版本只检查两维坐标范围。
proj、reflect 相对无限直线，nearest 返回点到线段的距离。Line.side 是带符号距离，有向直线左侧为正。
*/

#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

bool checkPll(P p1, P p2, P q1, P q2) {
    db a1 = cross(q1, q2, p1), a2 = - cross(q1, q2, p2);
    return sign(a1 + a2) == 0;
}

P isLL(P p1, P p2, P q1, P q2) {
    db a1 = cross(q1, q2, p1), a2 = - cross(q1, q2, p2);
    return (p1 * a2 + p2 * a1) / (a1 + a2);
}

bool intersect(db l1, db r1, db l2, db r2) {
    if (l1 > r1) swap(l1, r1);
    if (l2 > r2) swap(l2, r2);
    return cmp(r1, l2) >= 0 && cmp(r2, l1) >= 0;
}

bool isSS(P p1, P p2, P q1, P q2) {
    return intersect(p1.x, p2.x, q1.x, q2.x) &&
           intersect(p1.y, p2.y, q1.y, q2.y) &&
           crossop(p1, p2, q1) * crossop(p1, p2, q2) <= 0 &&
           crossop(q1, q2, p1) * crossop(q1, q2, p2) <= 0;
}

bool isSS_strict(P p1, P p2, P q1, P q2) {
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) < 0 &&
           crossop(q1, q2, p1) * crossop(q1, q2, p2) < 0;
}

bool isLS(P p1, P p2, P q1, P q2) {
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) <= 0;
}

bool isLS_strict(P p1, P p2, P q1, P q2) {
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) < 0;
}


bool isMiddle(db a, db m, db b) {
    return sign(a - m) == 0 || sign(b - m) == 0 || ((a < m) != (b < m));
}

bool isMiddle(P a, P m, P b) {
    return isMiddle(a.x, m.x, b.x) && isMiddle(a.y, m.y, b.y);
}

bool onSeg(P p1, P p2, P q) {
    return crossop(p1, p2, q) == 0 && isMiddle(p1, q, p2);
}

bool onSeg_strict(P p1, P p2, P q) {
    return crossop(p1, p2, q) == 0 &&
           sign((q - p1).dot(p1 - p2)) *
           sign((q - p2).dot(p1 - p2)) < 0;
}

P proj(P p1, P p2, P q) {
    P dir = p2 - p1;
    return p1 + (p2 - p1) * (dir.dot(q - p1) / (dir).abs2());
}

P reflect(P p1, P p2, P q) {
    return proj(p1, p2, q) * 2 - q;
}

db nearest(P p1, P p2, P q) {
    if (p1 == p2) return p1.distTo(q);
    P h = proj(p1, p2, q);
    if (isMiddle(p1, h, p2))
        return q.distTo(h);
    return min(p1.distTo(q), p2.distTo(q));
}

db disSS(P p1, P p2, P q1, P q2) {
    if (isSS(p1, p2, q1, q2)) return 0;
    db val1 = min(nearest(p1, p2, q1), nearest(p1, p2, q2));
    db val2 = min(nearest(q1, q2, p1), nearest(q1, q2, p2));
    return min(val1, val2);
}

struct Line {
    P p, v;
    db ang;
    Line(P a, P b) : p(a), v((b - a).unit()), ang(v.alpha()) {
        if (ang < 0) ang += 2 * acosl(-1);
    }
    db side(P x) { return v.det(x - p); }
};

/*
P a{0, 0}, b{2, 0}, c{1, -1}, d{1, 1};
if (! checkPll(a, b, c, d)) cout << isLL(a, b, c, d);
bool ss = isSS(a, b, c, d), proper = isSS_strict(a, b, c, d);
bool ls = isLS(a, b, c, d), crosses = isLS_strict(a, b, c, d);
bool overlap = intersect(0, 2, 1, 3), between = isMiddle(0, 1, 2);
bool in_box = isMiddle(a, c, b);
bool on = onSeg(a, b, c), interior = onSeg_strict(a, b, c);
P foot = proj(a, b, d), mirror = reflect(a, b, d);
db dist = nearest(a, b, d), gap = disSS(a, b, c, d);
Line line(a, b); db signed_dist = line.side(d);
*/
