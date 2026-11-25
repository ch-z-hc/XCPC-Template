#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

/*
先复制「点与向量」「直线与线段」「多边形」「凸包与直径与闵可夫斯基和」。
由任意点集建凸包后判断包含关系。允许重复、共线及空集。建凸包 O(n log n)，查询 O(log n)。
*/

struct ConvexQuery {
    V < P > a;
    void build(V < P > points) { a = convexHull(points); }
    int query(P p) { return containConvex(a, p); }
};

/*
ConvexQuery cq; cq.build({{0, 0}, {2, 0}, {0, 2}});
int state = cq.query({1, 0}); // 0 外部、1 边界、2 内部
*/
