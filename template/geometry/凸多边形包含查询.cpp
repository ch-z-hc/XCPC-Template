#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制本章的“点与向量”“直线与线段”“多边形”
// 和“凸包、直径与闵可夫斯基和”。
// build(points) 先求严格凸包，允许乱序、重复、共线和空点集。
// 预处理 O(n log n)，查询 O(log n)，空间 O(n)；采用原版 EPS 判断。
// query(p) 返回 0 外部 / 1 边界 / 2 内部，凸包顶点下标 0-based。
struct ConvexQuery {
    V < P > a;
    void build(V < P > points) { a = convexHull(points); }
    int query(P p) { return containConvex(a, p); }
};

// ConvexQuery cq; cq.build(points); int state = cq.query(p);
