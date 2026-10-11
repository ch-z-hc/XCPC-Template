#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制 Point.cpp、Line.cpp、Polygon.cpp、ConvexHull.cpp。
// build(points) 先求严格凸包，允许乱序、重复、共线和空点集。
// 预处理 O(n log n)，查询 O(log n)，空间 O(n)；采用原版 EPS 判断。
// query(p) 返回 0 外部 / 1 边界 / 2 内部，凸包顶点下标 0-based。
struct ConvexQuery {
    V < P > a;
    void build(V < P > points) { a = convexHull(points); }
    int query(P p) {
        int n = int(a.size());
        if (! n) return 0;
        if (n == 1) return a[0] == p;
        if (n == 2) return onSeg(a[0], a[1], p);
        int left = crossop(a[0], a[1], p), right = crossop(a[0], a[n - 1], p);
        if (left < 0 || right > 0) return 0;
        if (! left) return onSeg(a[0], a[1], p);
        if (! right) return onSeg(a[0], a[n - 1], p);
        int l = 1, r = n - 1;
        while (r - l > 1) {
            int mid = (l + r) >> 1;
            if (crossop(a[0], a[mid], p) >= 0) l = mid;
            else r = mid;
        }
        int d = crossop(a[l], a[r], p);
        return d < 0 ? 0 : d == 0 ? 1 : 2;
    }
};

// ConvexQuery cq; cq.build(points); int state = cq.query(p);
