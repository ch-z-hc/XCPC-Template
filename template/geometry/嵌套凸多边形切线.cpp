#include <bits/stdc++.h>

#define V vector

using namespace std;

// 先复制本章的“点与向量”，复用 P、db 和 cross。
// p、q 均为严格逆时针凸多边形，至少 3 点，无共线相邻边、无首尾重复。
// q 必须严格位于 p 内部；不检查前提，不接收无序点集。
// 直接比较叉积；坐标差与叉积须在 db 的精度 / 数值范围内。
// 旋转卡壳 O(n + m) 时间与空间（输入按值传入）；build 可重复调用。
// left[i] / right[i] 为 q 的 0-based 切点，范围 [0, m)。
// q 在 p[i] -> q[left[i]] 左侧，在 p[i] -> q[right[i]] 右侧，均含边界。
// 切线贴合 q 的一条边时，取该边逆时针方向的后一个端点。
// l、r 长度为 2n，存展开下标，不取模：r[i+n] = r[i] + n，l 同理。
// 对 0 <= i < n：i < r[i] < l[i] < i+n。
// i < j <= r[i] 时 q 全在 i -> j 左侧；l[i] <= j < i+n 时全在右侧。
// 其余弦穿过 q 内部；相切合法。访问点用 p[j % n]，区间比较不能取模。
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
            // 外点沿 p 逆时针移动，两侧切点均沿 q 逆时针移动。
            // 等号将贴边切线统一到后一个端点，避免平台阻塞指针。
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
