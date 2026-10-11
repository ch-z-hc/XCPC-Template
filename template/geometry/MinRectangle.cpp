#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制 Point.cpp；输入为严格逆时针凸包，无重复点、无首尾重复。
// 调用者可用 convexHull 取得凸包；不直接接收任意点集或顺时针多边形。
// 旋转卡壳 O(n) 时间、O(n) 空间（输入按值传入），下标 0-based。
// 返回最小面积及逆时针四角；空集返回空角点，1 / 2 点返回退化矩形。
// 不使用模数，坐标与中间点积、叉积须在 db 精度可用范围内。
struct MinRect {
    db area = 0;
    V < P > corners;
};

MinRect min_rectangle(V < P > ps) {
    int n = int(ps.size());
    if (! n) return {};
    if (n == 1) return {0, V < P > (4, ps[0])};
    if (n == 2) return {0, {ps[0], ps[1], ps[1], ps[0]}};
    int hi = 0, lo = 0, top = 0;
    P e = ps[1] - ps[0];
    for (int j = 1; j < n; ++ j) {
        if ((ps[j] - ps[0]).dot(e) > (ps[hi] - ps[0]).dot(e)) hi = j;
        if ((ps[j] - ps[0]).dot(e) < (ps[lo] - ps[0]).dot(e)) lo = j;
        if (e.det(ps[j] - ps[0]) > e.det(ps[top] - ps[0])) top = j;
    }
    MinRect ans{numeric_limits < db >::infinity(), {}};
    for (int i = 0; i < n; ++ i) {
        e = ps[(i + 1) % n] - ps[i];
        auto dot = [&](int j) { return (ps[j] - ps[i]).dot(e); };
        auto det = [&](int j) { return e.det(ps[j] - ps[i]); };
        while (dot((hi + 1) % n) > dot(hi)) hi = (hi + 1) % n;
        while (dot((lo + 1) % n) < dot(lo)) lo = (lo + 1) % n;
        while (det((top + 1) % n) > det(top)) top = (top + 1) % n;
        db l = dot(lo), r = dot(hi), h = max(0.L, det(top)), d = e.abs2();
        db area = (r - l) * h / d;
        if (area < ans.area) {
            P a = ps[i] + e * (l / d), b = ps[i] + e * (r / d);
            P v = e.rot90() * (h / d);
            ans = {area, {a, b, b + v, a + v}};
        }
    }
    return ans;
}

// auto rect = min_rectangle(hull); db ans = rect.area;
