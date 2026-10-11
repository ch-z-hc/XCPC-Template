#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 独立三维片段，不修改 Point.cpp 的 P / EPS；无模数。
// 增量凸包最坏 O(n² log n) 时间、O(n) 空间；面积 / 体积 O(n)。
// build(ps, eps) 保留输入顺序；p 和三角面 faces 均为 0-based。
// dim 为 -1 / 0 / 1 / 2 / 3：空 / 点 / 线 / 面 / 体。
// dim < 3 时 faces 为空，area / volume 均返回 0，不计算平面凸包面积。
// 支持重复点、共面面片；不扰动输入。eps 是距离容差，默认 1e-9。
// 适用：有限坐标、适中尺度，非零点线面距离应明显大于 eps 和舍入误差。
// 近退化或大坐标数据须缩放并调整 eps；此浮点实现不保证精确谓词。

struct P3 {
    db x = 0, y = 0, z = 0;
    P3 operator +(P3 p) const { return {x + p.x, y + p.y, z + p.z}; }
    P3 operator -(P3 p) const { return {x - p.x, y - p.y, z - p.z}; }
    P3 operator *(db d) const { return {x * d, y * d, z * d}; }
    P3 operator /(db d) const { return {x / d, y / d, z / d}; }
    db dot(P3 p) const { return x * p.x + y * p.y + z * p.z; }
    P3 det(P3 p) const {
        return {y * p.z - z * p.y, z * p.x - x * p.z,
                x * p.y - y * p.x};
    }
    db abs() const { return sqrtl(dot(*this)); }
};

struct Hull3D {
    struct Face { int a, b, c; };
    V < P3 > p;
    V < Face > faces;
    P3 inside;
    int dim = -1;
    db eps = 1e-9;

    P3 normal(Face f) const {
        return (p[f.b] - p[f.a]).det(p[f.c] - p[f.a]);
    }

    void add_face(int a, int b, int c) {
        Face f{a, b, c};
        if (normal(f).dot(inside - p[a]) > 0) swap(f.b, f.c);
        faces.push_back(f);
    }

    void build(const V < P3 > &ps, db tolerance = 1e-9) {
        p = ps; faces.clear(); eps = tolerance; dim = -1;
        int n = p.size();
        if (! n) return;
        dim = 0;
        int b = 0, c = 0, d = 0;
        for (int i = 1; i < n; ++ i)
            if ((p[i] - p[0]).abs() > (p[b] - p[0]).abs()) b = i;
        if ((p[b] - p[0]).abs() <= eps) return;
        dim = 1;
        P3 u = p[b] - p[0];
        for (int i = 0; i < n; ++ i)
            if (u.det(p[i] - p[0]).abs() >
                u.det(p[c] - p[0]).abs()) c = i;
        if (u.det(p[c] - p[0]).abs() <= eps * u.abs()) return;
        dim = 2;
        P3 v = u.det(p[c] - p[0]);
        for (int i = 0; i < n; ++ i)
            if (fabsl(v.dot(p[i] - p[0])) >
                fabsl(v.dot(p[d] - p[0]))) d = i;
        if (fabsl(v.dot(p[d] - p[0])) <= eps * v.abs()) return;
        dim = 3; inside = (p[0] + p[b] + p[c] + p[d]) / 4;
        add_face(0, b, c); add_face(0, b, d);
        add_face(0, c, d); add_face(b, c, d);
        for (int i = 0; i < n; ++ i) {
            if (i == 0 || i == b || i == c || i == d) continue;
            set < pair < int, int > > edge;
            V < Face > keep;
            for (Face f : faces) {
                P3 w = normal(f);
                if (w.dot(p[i] - p[f.a]) <= eps * w.abs()) {
                    keep.push_back(f); continue;
                }
                for (auto [x, y] : {pair{f.a, f.b},
                                    pair{f.b, f.c}, pair{f.c, f.a}}) {
                    auto it = edge.find({y, x});
                    if (it != edge.end()) edge.erase(it);
                    else edge.insert({x, y});
                }
            }
            faces.swap(keep);
            for (auto [x, y] : edge) add_face(x, y, i);
        }
    }

    db area() const {
        db ans = 0;
        for (Face f : faces) ans += normal(f).abs() / 2;
        return ans;
    }

    db volume() const {
        db ans = 0;
        for (Face f : faces)
            ans += normal(f).dot(p[f.a] - inside) / 6;
        return fabsl(ans);
    }
};

// Hull3D hull; hull.build(ps); hull.area(); hull.volume();
// hull.faces 中每个三角面的顶点按外法向排列。
