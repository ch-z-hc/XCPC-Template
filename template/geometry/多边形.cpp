#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制本章的“点与向量”“直线与线段”。
// 简单多边形按边界顺序给点，0-based。
// area、contain、convexCut 时间 O(n)；convexCut 要求输入为凸多边形。
// 浮点采用 EPS；area 是有向面积，逆时针为正，切割保留直线左侧。
// area 以首点为原点减少大坐标相消；仍须保证输入坐标能保留所需精度。

db area(V < P > ps) {
    // 求多边形面积（逆时针顺序给出点 -> 面积为正）
    db ret = 0; if (ps.size() == 0) return 0;
    for (int i = 1; i + 1 < (int)ps.size(); ++ i) {
        ret += (ps[i] - ps[0]).det(ps[i + 1] - ps[0]);
    }
    return ret / 2;
}

int contain(V < P > ps, P p) {
    // 0 : outside
    // 1 : on_seg
    // 2 : inside

    // ps 是多边形，判断 p 在多边形 内部 / 线段上 / 外部
    int n = ps.size(), ret = 0;
    for (int i = 0; i < n; ++ i) {
        P u = ps[i], v = ps[(i + 1) % n];
        if (onSeg(u, v, p)) return 1;
        if (cmp(u.y, v.y) <= 0) swap(u, v);
        if (cmp(p.y, u.y) > 0 || cmp(p.y, v.y) <= 0)
            continue;
        ret ^= crossop(p, u, v) > 0;
    }
    return ret * 2;
}

V < P > convexCut(V<P> ps, P q1, P q2) {
    // 直线切割凸包，返回直线左边凸包的点
    V < P > qs;
    int n = ps.size();
    for (int i = 0; i < n; ++ i) {
        P p1 = ps[i], p2 = ps[(i + 1) % n];
        int d1 = crossop(q1, q2, p1), d2 = crossop(q1, q2, p2);
        if (d1 >= 0)
            qs.push_back(p1);
        if (d1 * d2 < 0)
            qs.push_back(isLL(p1, p2, q1, q2));
    }
    return qs;
}
