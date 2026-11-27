/*
先复制「二维凸包最大点积查询」。动态维护整点集，坐标和方向限制同 HullQuery。
设 Q 为累计插入次数，块长 B > 0。增删均摊 O(B log(B + 1))，点积 O(ceil(Q/B) log(B + 1))。
取完整凸包和单次包含查询均为 O(Q log(Q + 1))。连续查包含可共用一次快照。
erase 按插入编号删除，重复删除返回 false。空集的点积查询返回 false。
contains 返回 0、1、2，分别表示外部、边界、内部。snapshot 返回当前全部点的静态凸包。
*/

#include <bits/stdc++.h>

#define V vector
using namespace std;

struct DynamicHull {
    typedef HullQuery::P P;
    int B;
    V < P > p;
    V < int > op;
    V < HullQuery > tr;

    DynamicHull(int b = 700) { init(b); }

    void init(int b = 700) {
        assert(b > 0);
        B = b;
        p.clear(); op.clear();
        tr.clear();
    }

    void rebuild(int k) {
        V < P > a;
        int l = k * B, r = min((int)p.size(), l + B);
        for (int i = l; i < r; i ++)
            if (op[i]) a.push_back(p[i]);
        tr[k].build(a);
    }

    int add(P q) {
        int id = p.size(), k = id / B;
        p.push_back(q);
        op.push_back(1);
        if (k == (int)tr.size()) tr.emplace_back();
        rebuild(k);
        return id;
    }

    bool erase(int id) {
        if (id < 0 || id >= (int)p.size() || !op[id]) return 0;
        op[id] = 0;
        rebuild(id / B);
        return 1;
    }

    bool query(ll a, ll b, ll &ans) {
        bool ok = 0;
        for (auto &t : tr) {
            ll v;
            if (!t.query(a, b, v)) continue;
            if (!ok || v > ans) ans = v;
            ok = 1;
        }
        return ok;
    }

    HullQuery snapshot() {
        V < P > a;
        for (auto &t : tr) a.insert(a.end(), t.h.begin(), t.h.end());
        HullQuery hull;
        hull.build(a);
        return hull;
    }

    int contains(P q) {
        HullQuery hull = snapshot();
        return hull.contains(q);
    }
};

/*
DynamicHull hull; hull.init(32);
int id = hull.add({2, 3}); hull.add({-1, 4});
ll ans; if (hull.query(1, 1, ans)) cout << ans;
int state = hull.contains({2, 3});
auto now = hull.snapshot();
bool ok = hull.erase(id);
*/
