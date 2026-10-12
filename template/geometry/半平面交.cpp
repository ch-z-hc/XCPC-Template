#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制本章的“点与向量”“直线与线段”。
// 半平面交，时间 O(n log n)，空间 O(n)。直线均保留左侧。
// 返回有正面积的有界交集，顶点逆时针；无界、无解或退化为线 / 点均返回空。
// 需要限制范围时，显式添加边界矩形。
V < P > half_plane(V < Line > lines) {
    sort(lines.begin(), lines.end(), [](const Line &a, const Line &b) {
        return a.ang < b.ang;
    });
    V < Line > a;
    for (auto line : lines) {
        if (! a.empty() && abs(a.back().v.det(line.v)) <= EPS &&
            a.back().v.dot(line.v) > 0) {
            if (a.back().side(line.p) > 0) a.back() = line;
        } else a.push_back(line);
    }
    if (a.size() >= 2 && abs(a.front().v.det(a.back().v)) <= EPS &&
        a.front().v.dot(a.back().v) > 0) {
        if (a.front().side(a.back().p) > 0) a.front() = a.back();
        a.pop_back();
    }
    auto intersect = [](Line a, Line b) {
        return a.p + a.v * ((b.p - a.p).det(b.v) / a.v.det(b.v));
    };
    deque < Line > q;
    for (auto line : a) {
        while (q.size() >= 2 && line.side(intersect(q[q.size() - 2], q.back())) < -EPS)
            q.pop_back();
        while (q.size() >= 2 && line.side(intersect(q[0], q[1])) < -EPS)
            q.pop_front();
        if (! q.empty() && abs(q.back().v.det(line.v)) <= EPS) return {};
        q.push_back(line);
    }
    while (q.size() >= 3 && q[0].side(intersect(q[q.size() - 2], q.back())) < -EPS)
        q.pop_back();
    while (q.size() >= 3 && q.back().side(intersect(q[0], q[1])) < -EPS)
        q.pop_front();
    V < P > ans;
    if (q.size() < 3) return ans;
    for (int i = 0; i < int(q.size()); ++ i) {
        Line u = q[i], v = q[(i + 1) % q.size()];
        if (u.v.det(v.v) <= EPS) return {};
        ans.push_back(intersect(u, v));
    }
    db area = 0;
    for (int i = 1; i + 1 < int(ans.size()); ++ i)
        area += (ans[i] - ans[0]).det(ans[i + 1] - ans[0]);
    return area > EPS ? ans : V < P >{};
}
