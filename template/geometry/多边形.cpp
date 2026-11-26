/*
先复制「点与向量」「直线与线段」。简单多边形按边界顺序给点，坐标须保留所需精度。
有向面积、一般包含查询、凸多边形切割 O(n)。有序严格凸多边形的包含查询 O(log n)。
area 逆时针为正。contain 和 containConvex 返回 0、1、2，分别表示外部、边界、内部。
containConvex 接受顺、逆时针严格凸多边形，末尾不重复首点，也接受空集、单点和线段。
convexCut 保留凸多边形在有向直线 q1 -> q2 左侧的部分，含边界。
*/

#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

db area(V < P > ps) {
    db ret = 0; if (ps.size() == 0) return 0;
    for (int i = 1; i + 1 < (int)ps.size(); ++ i) {
        ret += (ps[i] - ps[0]).det(ps[i + 1] - ps[0]);
    }
    return ret / 2;
}

int contain(V < P > ps, P p) {
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

int containConvex(const V < P > &ps, P p) {
    int n = ps.size();
    if (! n) return 0;
    if (n == 1) return ps[0] == p;
    if (n == 2) return onSeg(ps[0], ps[1], p);

    P o = ps[0];
    int l = 1, r = n - 1;
    if (cross(o, ps[l], ps[r]) < 0) swap(l, r);
    if (onSeg(o, ps[l], p) || onSeg(o, ps[r], p)) return 1;
    if (cross(o, ps[l], p) <= 0 || cross(o, ps[r], p) >= 0) return 0;

    while (abs(r - l) > 1) {
        int mid = l + (r - l) / 2;
        if (cross(o, ps[mid], p) >= 0) l = mid;
        else r = mid;
    }
    return crossop(ps[l], ps[r], p) + 1;
}

V < P > convexCut(V<P> ps, P q1, P q2) {
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

/*
V < P > ps{{0, 0}, {3, 0}, {0, 3}}; P q{1, 1};
db signed_area = area(ps);
int state = contain(ps, q), fast = containConvex(ps, q);
auto cut = convexCut(ps, {1, 0}, {1, 1});
*/
