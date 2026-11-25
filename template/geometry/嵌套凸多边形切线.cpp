#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
先复制「点与向量」。对外多边形各顶点求内多边形切点及不穿过其内部的弦，O(n + m)。
两多边形均严格凸、逆时针、至少 3 点，末尾不重复首点，内多边形严格在外多边形内部。
直接比较叉积，坐标差和中间乘积须在 db 的精度范围内。
*/

struct NestedHull {
    V < int > left, right, l, r;

    void build(V < P > p, V < P > q) {
        int n = p.size(), m = q.size();
        left.resize(n); right.resize(n);
        l.resize(2 * n); r.resize(2 * n);
        int a = 0, b = 0;
        // 从外部点看 q 的方向落在不足半圈内，可线性找两端。
        for (int j = 1; j < m; ++ j) {
            if (cross(p[0], q[a], q[j]) < 0) a = j;
            if (cross(p[0], q[b], q[j]) > 0) b = j;
        }
        for (int i = 0; i < n; ++ i) {
            /*
            外点沿 p 逆时针移动，两侧切点均沿 q 逆时针移动。
            等号将贴边切线统一到后一个端点，避免平台阻塞指针。
            */
            while (cross(p[i], q[a], q[(a + 1) % m]) <= 0)
                a = (a + 1) % m;
            while (cross(p[i], q[b], q[(b + 1) % m]) >= 0)
                b = (b + 1) % m;
            left[i] = a; right[i] = b;
        }
        int j = 1;
        for (int i = 0; i < n; ++ i) {
            j = max(j, i + 1);
            while (j + 1 < i + n &&
                   cross(p[i], p[(j + 1) % n], q[left[i]]) >= 0) ++ j;
            r[i] = j;
            r[i + n] = j + n;
        }
        j = 1;
        for (int i = 0; i < n; ++ i) {
            j = max(j, i + 1);
            // q 在 i -> j 右侧，等价于在 j -> i+n 左侧。
            while (r[j] < i + n) ++ j;
            l[i] = j;
            l[i + n] = j + n;
        }
    }
};

/*
V < P > p{{0, 0}, {6, 0}, {6, 6}, {0, 6}}, q{{2, 2}, {4, 2}, {3, 4}};
NestedHull hull; hull.build(p, q);
int i = 0, n = p.size();
P a = q[hull.left[i]], b = q[hull.right[i]]; // q 在 p[i] -> a 左侧、p[i] -> b 右侧
// 贴边切点取逆时针后端点。弦 i -> j 用展开下标判断，仅在取点时对 n 取模。
for (int j = i + 1; j <= hull.r[i]; ++ j) cout << p[j % n]; // q 全在弦左侧，含边界
for (int j = hull.l[i]; j < i + n; ++ j) cout << p[j % n]; // q 全在弦右侧，含边界
*/
