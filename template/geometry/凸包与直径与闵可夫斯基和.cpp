/*
先复制「点与向量」，闵可夫斯基和还需「多边形」的 area。
建凸包 O(n log n)，判凸和直径 O(n)，闵可夫斯基和 O(n + m)。
直径与闵可夫斯基和接受顺、逆时针凸包，相邻顶点不重复，末尾不重复首点。
convexHull 只留极点，convexHullNonStrict 保留边上的共线点，全共线时中间点会往返各出现一次。
checkConvexHull 要求按边界顺序输入简单多边形，判断逆时针凸性，允许相邻边共线。
MincowskySum 任一输入为空时返回空集。
*/

#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

V < P > convexHull(V < P > ps) {
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
    merge(p.begin(), p.end(), q.begin(), q.end(),
          qs.begin() + 1, polar_cmp({0, -1}, false));
    for (int i = 1; i < (int)qs.size(); ++ i)
        qs[i] = qs[i] + qs[i - 1];
    qs.pop_back();
    return qs;
}

/*
V < P > ps{{0, 0}, {1, 0}, {2, 0}, {0, 2}};
auto h = convexHull(ps), border = convexHullNonStrict(ps);
bool ccw = checkConvexHull(h);
db diameter = convexDiameter(h);
auto sum = MincowskySum(h, h);
*/
