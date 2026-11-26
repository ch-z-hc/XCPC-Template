/*
先复制「点与向量」。对外多边形各顶点求内多边形切点及不穿过其内部的弦，O(n + m)。
两多边形均严格凸、逆时针、至少 3 点，末尾不重复首点，内多边形严格在外多边形内部。
直接比较叉积，坐标差和中间乘积须在 db 的精度范围内。
q 在 p[i] 指向 left[i] 切点的左侧、指向 right[i] 切点的右侧。贴边切点取逆时针后端点。
弦 i -> j 在 i < j <= r[i] 时将 q 留在左侧，在 l[i] <= j < i + n 时将 q 留在右侧，均含边界。
j 使用展开下标，取点时才对 n 取模。
*/

#include <bits/stdc++.h>

struct NestedHull {
    std::vector<int> left, right, l, r;

    void build(std::vector<P> p, std::vector<P> q) {
        int n = p.size(), m = q.size();
        left.resize(n);
        right.resize(n);
        l.resize(2 * n);
        r.resize(2 * n);
        int a = 0, b = 0;
        for (int j = 1; j < m; j ++) {
            if (cross(p[0], q[a], q[j]) < 0) a = j;
            if (cross(p[0], q[b], q[j]) > 0) b = j;
        }
        for (int i = 0; i < n; i ++) {
            while (cross(p[i], q[a], q[(a + 1) % m]) <= 0) a = (a + 1) % m;
            while (cross(p[i], q[b], q[(b + 1) % m]) >= 0) b = (b + 1) % m;
            left[i] = a;
            right[i] = b;
        }
        int j = 1;
        for (int i = 0; i < n; i ++) {
            j = std::max(j, i + 1);
            while (j + 1 < i + n && cross(p[i], p[(j + 1) % n], q[left[i]]) >= 0) ++ j;
            r[i] = j;
            r[i + n] = j + n;
        }
        j = 1;
        for (int i = 0; i < n; i ++) {
            j = std::max(j, i + 1);
            while (r[j] < i + n) ++ j;
            l[i] = j;
            l[i + n] = j + n;
        }
    }
};

/*
std::vector<P> p{{0, 0}, {6, 0}, {6, 6}, {0, 6}}, q{{2, 2}, {4, 2}, {3, 4}};
NestedHull hull; hull.build(p, q);
int i = 0, n = p.size();
P a = q[hull.left[i]], b = q[hull.right[i]];
for (int j = i + 1; j <= hull.r[i]; j ++) cout << p[j % n];
for (int j = hull.l[i]; j < i + n; j ++) cout << p[j % n];
*/
