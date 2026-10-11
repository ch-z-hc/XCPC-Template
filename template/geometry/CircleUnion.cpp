#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制 Point.cpp、Line.cpp、Circle.cpp；有限坐标、非负半径，无模数。
// 多圆面积并 / 周长并，O(n² log n) 时间、O(n) 额外空间。
// circle_union(cs) 返回 {面积, 周长}；空集、零半径圆贡献为 0。
// 相同圆只计一次，支持包含、内外相切、分离和有洞的圆并。
// 只积分未被覆盖的圆弧，周长包含洞的边界；不把相切看作有长覆盖。
// 包含判定不额外加 EPS，避免误删近乎重合的小圆；acos 参数截断。
// 适中尺度浮点数据使用 long double；极近退化不保证精确拓扑。

pair < db, db > circle_union(const V < Circle > &cs) {
    const db pi = acosl(-1), tau = 2 * pi;
    int n = cs.size();
    db area = 0, perimeter = 0;
    if (! n) return {0, 0};
    // 平移积分原点，减少远离原点时的大数相消。
    P origin = cs[0].o;
    for (int i = 0; i < n; ++ i) {
        Circle a = cs[i];
        if (a.r == 0) continue;
        V < pair < db, db > > covered;
        bool hidden = 0;
        for (int j = 0; j < n; ++ j) {
            if (i == j || cs[j].r == 0) continue;
            Circle b = cs[j];
            P v = b.o - a.o;
            db d = v.abs();
            if (d == 0 && a.r == b.r) {
                if (j < i) { hidden = 1; break; }
                continue;
            }
            if (d + a.r <= b.r) { hidden = 1; break; }
            if (d >= a.r + b.r || d + b.r <= a.r) continue;
            db mid = atan2l(v.y, v.x);
            db angle = acosl(clamp((d * d + a.r * a.r - b.r * b.r) /
                                  (2 * d * a.r), -1.L, 1.L));
            db l = mid - angle, r = mid + angle;
            while (l < 0) { l += tau; r += tau; }
            while (l >= tau) { l -= tau; r -= tau; }
            if (r <= tau) covered.push_back({l, r});
            else {
                covered.push_back({l, tau});
                covered.push_back({0, r - tau});
            }
        }
        if (hidden) continue;
        sort(covered.begin(), covered.end());
        P o = a.o - origin;
        auto add_arc = [&](db l, db r) {
            if (l >= r) return;
            perimeter += a.r * (r - l);
            area += (a.r * a.r * (r - l) +
                     a.r * o.x * (sinl(r) - sinl(l)) +
                     a.r * o.y * (cosl(l) - cosl(r))) / 2;
        };
        db last = 0;
        for (auto [l, r] : covered) {
            add_arc(last, l); last = max(last, r);
        }
        add_arc(last, tau);
    }
    return {max(0.L, area), perimeter};
}

// auto [area, perimeter] = circle_union(cs);
