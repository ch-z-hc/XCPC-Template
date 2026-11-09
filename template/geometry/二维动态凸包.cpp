#include <bits/stdc++.h>

#define V vector

using namespace std;

// 先复制“二维凸包最大点积查询”；整数范围与 HullQuery 相同。
// 在线分块：add(p) 返回 0-based 插入编号，重复点分别保留。
// erase(id) 按编号删除；非法编号或重复删除返回 0。
// query(a, b, ans) 求最大点积；空集返回 0，ans 不可用。
// init(B) 清空状态和编号；B > 0，通常取预计操作数的平方根。
// Q 为累计插入次数（含已删除点），更新摊还 O(B log(B + 1))，空间 O(Q)。
// query 为 O(ceil(Q / B) * log(B + 1))。
// snapshot / contains 重建全局凸包，O(Q log(Q + 1))。
// 连续包含查询可复用 snapshot，再以 O(log Q) 调用其 contains。

struct DynamicHull {
    typedef HullQuery::P P;
    int B;
    V < P > p;
    V < int > op;
    V < HullQuery > tr;

    DynamicHull(int b = 700) { init(b); }

    void init(int b = 700) {
        assert(b > 0);
        B = b; p.clear(); op.clear(); tr.clear();
    }

    void rebuild(int k) {
        V < P > a;
        int l = k * B, r = min((int)p.size(), l + B);
        // 保留原点，删除外层顶点后内部点可能重新成为极点。
        for (int i = l; i < r; ++ i)
            if (op[i]) a.push_back(p[i]);
        tr[k].build(a);
    }

    int add(P q) {
        int id = p.size(), k = id / B;
        p.push_back(q); op.push_back(1);
        if (k == (int)tr.size()) tr.emplace_back();
        rebuild(k);
        return id;
    }

    bool erase(int id) {
        if (id < 0 || id >= (int)p.size() || ! op[id]) return 0;
        op[id] = 0; rebuild(id / B);
        return 1;
    }

    bool query(ll a, ll b, ll &ans) {
        bool ok = 0;
        for (auto &t : tr) {
            ll v;
            if (! t.query(a, b, v)) continue;
            if (! ok || v > ans) ans = v;
            ok = 1;
        }
        return ok;
    }

    HullQuery snapshot() {
        V < P > a;
        for (auto &t : tr) a.insert(a.end(), t.h.begin(), t.h.end());
        HullQuery hull; hull.build(a);
        return hull;
    }

    int contains(P q) {
        // 全局凸包不等于各块凸包的并。
        HullQuery hull = snapshot();
        return hull.contains(q);
    }
};

// DynamicHull hull; int id = hull.add({2, 3}); hull.add({-1, 4});
// ll ans; if (hull.query(1, 1, ans)) cout << ans << '\n'; // 5
// hull.erase(id); // 再查询为 3；重复删除返回 0。
// auto now = hull.snapshot(); // now.h 为当前逆时针凸包顶点。
