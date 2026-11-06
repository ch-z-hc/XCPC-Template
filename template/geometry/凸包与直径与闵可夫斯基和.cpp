#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制本章的“点与向量”；MincowskySum 还需“多边形”的 area。
// 凸包 O(n log n)，直径 O(n)，闵可夫斯基和 O(n + m)。
// 结果 0-based；直径要求凸包顶点按边界顺序，直径与闵可夫斯基和允许两种方向。
// 直径的相邻顶点不得重复，末尾不重复首点；允许空集、单点与线段。
// checkConvexHull 为 O(n)，只判断简单多边形的逆时针凸性，不检测自交。

V < P > convexHull(V < P > ps) {
    // 严格凸包，不保留边上的中间点。
    sort(ps.begin(), ps.end());
    ps.erase(unique(ps.begin(), ps.end(), [](P a, P b) {
        return a.x == b.x && a.y == b.y;
    }), ps.end());
    int n = ps.size();
    if (n <= 1) return ps;
    V < P > qs(n * 2);
    int k = 0;
    for (int i = 0; i < n; qs[k ++] = ps[i ++])
        while (k > 1 && crossop(qs[k - 2], qs[k - 1], ps[i]) <= 0)
            -- k;
    for (int i = n - 2, t = k; i >= 0; qs[k ++] = ps[i --])
        while (k > t && crossop(qs[k - 2], qs[k - 1], ps[i]) <= 0)
            -- k;
    qs.resize(k - 1);
    return qs;
}


V < P > convexHullNonStrict(V < P > ps) {
    // 凸包，保留边上的点。
    sort(ps.begin(), ps.end());
    ps.erase(unique(ps.begin(), ps.end(), [](P a, P b) {
        return a.x == b.x && a.y == b.y;
    }), ps.end());
    int n = ps.size();
    if (n <= 1) return ps;
    V < P > qs(n * 2);
    int k = 0;
    for (int i = 0; i < n; qs[k ++] = ps[i ++])
        while (k > 1 && crossop(qs[k - 2], qs[k - 1], ps[i]) < 0)
            -- k;
    for (int i = n - 2, t = k; i >= 0; qs[k ++] = ps[i --])
        while (k > t && crossop(qs[k - 2], qs[k - 1], ps[i]) < 0)
            -- k;
    qs.resize(k - 1);
    return qs;
}

bool checkConvexHull(V < P > p) {
    // 判断简单多边形是否逆时针凸，允许共线边界点。
    int n = p.size();
    for (int i = 0; i < n; ++ i) {
        int pre = i - 1;
        int suf = i + 1;
        if (pre == -1) pre = n - 1;
        if (suf == n) suf = 0;
        if (crossop(p[i], p[suf], p[pre]) < 0)
            return 0;
    }
    return 1;
}

db convexDiameter(V < P > ps) {
    // 旋转卡壳求凸包直径。
    int n = ps.size();
    if (n <= 1) return 0;
    db s = 0;
    for (int k = 1; k + 1 < n; ++ k)
        s += (ps[k] - ps[0]).det(ps[k + 1] - ps[0]);
    if (s < 0) reverse(ps.begin(), ps.end());
    int is = 0, js = 0;
    for (int k = 1; k < n; ++ k)
        is = ps[k] < ps[is] ? k : is,
        js = ps[js] < ps[k] ? k : js;
    int i = is, j = js;
    db ret = ps[i].distTo(ps[j]);
    do {
        if ((ps[(i + 1) % n] - ps[i]).det(ps[(j + 1) % n] - ps[j]) >= 0)
            (++ j) %= n;
        else (++ i) %= n;
        ret = max(ret, ps[i].distTo(ps[j]));
    } while (i != is || j != js);
    return ret;
}

V < P > MincowskySum(V < P > p, V < P > q) {
    // p、q 为凸包，顺 / 逆时针均可。
    // 任一输入为空集时，和集也为空。
    if (p.empty() || q.empty()) return {};
    auto normalize = [](V < P > & ps) {
        if (area(ps) < 0) reverse(ps.begin(), ps.end());
        rotate(ps.begin(), min_element(ps.begin(), ps.end()), ps.end());
    };
    normalize(p); normalize(q);
    V < P > qs; qs.push_back(p[0] + q[0]);
    P p0 = p[0], q0 = q[0];
    for (int i = 0; i + 1 < (int)p.size(); ++ i)
        p[i] = p[i + 1] - p[i];
    for (int i = 0; i + 1 < (int)q.size(); ++ i)
        q[i] = q[i + 1] - q[i];
    p[p.size() - 1] = p0 - p[p.size() - 1];
    q[q.size() - 1] = q0 - q[q.size() - 1];
    qs.resize(p.size() + q.size() + 1);
    merge(p.begin(), p.end(), q.begin(), q.end(), qs.begin() + 1, polar_cmpY);
    for (int i = 1; i < (int)qs.size(); ++ i)
        qs[i] = qs[i] + qs[i - 1];
    qs.pop_back();
    return qs;
}
