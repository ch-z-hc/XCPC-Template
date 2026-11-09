#include <bits/stdc++.h>

#define V vector

using namespace std;

// 先复制“二维凸包最大点积查询”；整数范围与 HullQuery 相同。
// 在线分块，支持任意顺序插入、删除与最大点积查询，不需预知操作。
// add(p) 返回从 0 开始的插入编号；重复点分别计数，删除用编号。
// erase(id) 删除仍存在的点，非法编号或重复删除返回 false。
// init(B) 清空全部状态和编号；B > 0，通常取预计操作数的平方根。
// Q 为 init 后累计插入次数（含已删除点），B 为块长。
// 更新摊还 O(B log(B + 1))，query O(ceil(Q / B) * log(B + 1))，空间 O(Q)。
// 分块保留所有原点，删除凸包顶点后原内部点可重新成为顶点。
// snapshot() 返回整个当前点集的 HullQuery，O(Q log(Q + 1))。
// contains 同样重建全局凸包，不是对各块包含结果取并；大量包含查询
// 且中间无更新时，先取得 snapshot，再用其 O(log Q) 的 contains。

struct DynamicHull {
    typedef HullQuery::P P;
    int B = 700;
    V < P > p;
    V < bool > alive;
    V < HullQuery > block;

    DynamicHull(int size = 700) { init(size); }

    void init(int size = 700) {
        assert(size > 0);
        B = size; p.clear(); alive.clear(); block.clear();
    }

    void rebuild(int k) {
        V < P > points;
        int l = k * B, r = min((int)p.size(), l + B);
        for (int i = l; i < r; ++ i)
            if (alive[i]) points.push_back(p[i]);
        block[k].build(points);
    }

    int add(P q) {
        int id = p.size(), k = id / B;
        p.push_back(q); alive.push_back(true);
        if (k == (int)block.size()) block.emplace_back();
        rebuild(k);
        return id;
    }

    bool erase(int id) {
        if (id < 0 || id >= (int)p.size() || ! alive[id]) return false;
        alive[id] = false;
        rebuild(id / B);
        return true;
    }

    bool query(ll a, ll b, ll &ans) {
        bool found = false;
        for (auto &part : block) {
            ll value;
            if (! part.query(a, b, value)) continue;
            if (! found || value > ans) ans = value;
            found = true;
        }
        return found;
    }

    HullQuery snapshot() {
        V < P > points;
        for (auto &part : block)
            points.insert(points.end(), part.h.begin(), part.h.end());
        HullQuery hull; hull.build(points);
        return hull;
    }

    int contains(P q) {
        HullQuery hull = snapshot();
        return hull.contains(q);
    }
};

// DynamicHull hull; int id = hull.add({2, 3}); hull.add({-1, 4});
// ll ans; if (hull.query(1, 1, ans)) { /* ans = 5 */ }
// hull.erase(id); // 再查询为 3；重复删除返回 false。
// auto now = hull.snapshot(); // now.h 为当前逆时针凸包顶点。
