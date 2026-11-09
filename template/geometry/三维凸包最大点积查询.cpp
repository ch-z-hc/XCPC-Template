#include <bits/stdc++.h>

#define V vector

using namespace std;

// 先复制“三维凸包”，使用其 P3 / db / Hull3D；静态点集，可重复 build。
// query(a, b, c, ans) 求 max(a * x + b * y + c * z)。
// 空集返回 0，ans 不可用；成功后 pos 是一个最优点的原输入 0-based 编号。
// 建凸包最坏 O(n² log n)，空间 O(n)；体凸包查询最坏 O(h)，h 为三角面所用点数。
// 从上次答案沿边上升，无对数复杂度保证；点 / 线 / 面退化时扫描原点 O(n)。
// 浮点精度与 Hull3D 相同：有限坐标、适中尺度，容差默认 1e-9。
// eps 可能忽略近表面的点，不能用于要求原点集严格精确答案的整点题。
// 查询系数也须有限，点积不能溢出；大系数会放大建凸包的几何误差。
// 等值区域递归最深 O(h)，大数据需留足栈空间。

struct HullQuery3D {
    Hull3D hull;
    V < V < int > > g;
    V < int > op, used;
    int pos = -1;

    void build(const V < P3 > &p, db eps = 1e-9) {
        hull.build(p, eps);
        g.assign(p.size(), {});
        op.assign(p.size(), 0); used.clear();
        pos = p.empty() ? -1 : 0;
        if (hull.dim < 3) return;
        pos = hull.faces[0].a;
        for (auto f : hull.faces)
            for (auto [u, v] : {pair{f.a, f.b},
                                pair{f.b, f.c}, pair{f.c, f.a}}) {
                g[u].push_back(v); g[v].push_back(u);
            }
    }

    int climb(int u, P3 q, db val) {
        op[u] = 1; used.push_back(u);
        for (int v : g[u]) {
            db d = hull.p[v].dot(q);
            if (d > val) return v;
            if (d == val && ! op[v]) {
                int id = climb(v, q, val);
                if (id != -1) return id;
            }
        }
        return -1;
    }

    bool query(db a, db b, db c, db &ans) {
        if (pos == -1) return 0;
        P3 q{a, b, c};
        ans = hull.p[pos].dot(q);
        if (hull.dim < 3) {
            for (int i = 0; i < (int)hull.p.size(); ++ i) {
                db v = hull.p[i].dot(q);
                if (v > ans) ans = v, pos = i;
            }
            return 1;
        }
        // 面片可能保留非极点，须穿过等值区域再找上升边。
        // 每点至多访问一次，比较不加 eps，避免零方向和等值面上循环。
        while (1) {
            int id = climb(pos, q, ans);
            if (id == -1) break;
            pos = id; ans = hull.p[pos].dot(q);
        }
        for (int i : used) op[i] = 0;
        used.clear();
        return 1;
    }
};

// HullQuery3D hull; hull.build({{0, 0, 0}, {2, 0, 0}, {0, 3, 0}, {0, 0, 4}});
// db ans; if (hull.query(1, 2, -1, ans)) cout << ans << '\n'; // 6
