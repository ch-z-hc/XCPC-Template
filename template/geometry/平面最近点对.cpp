/*
先复制「点与向量」。平面最近点对 O(n log n)，坐标有限，允许重复点。
build 返回原数组下标，d2 为距离平方。少于两点时返回 {-1, -1}。
*/

#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

struct ClosestPair {
    struct Node { P p; int id; };
    V < Node > a, tmp;
    pair < int, int > ids;
    db d2;

    void update(Node u, Node v) {
        db d = (u.p - v.p).abs2();
        if (d < d2) d2 = d, ids = {u.id, v.id};
    }
    void dfs(int l, int r) {
        if (r - l <= 3) {
            for (int i = l; i < r; ++ i)
                for (int j = i + 1; j < r; ++ j) update(a[i], a[j]);
            sort(a.begin() + l, a.begin() + r, [](Node u, Node v) {
                return u.p.y < v.p.y;
            });
            return;
        }
        int mid = (l + r) >> 1;
        db x = a[mid].p.x;
        dfs(l, mid); dfs(mid, r);
        merge(a.begin() + l, a.begin() + mid, a.begin() + mid, a.begin() + r,
              tmp.begin() + l, [](Node u, Node v) { return u.p.y < v.p.y; });
        copy(tmp.begin() + l, tmp.begin() + r, a.begin() + l);
        int cnt = 0;
        for (int i = l; i < r; ++ i) if ((a[i].p.x - x) * (a[i].p.x - x) < d2) {
            for (int j = cnt - 1; j >= 0; -- j) {
                db y = a[i].p.y - tmp[l + j].p.y;
                if (y * y >= d2) break;
                update(a[i], tmp[l + j]);
            }
            tmp[l + cnt ++] = a[i];
        }
    }
    pair < int, int > build(const V < P > &points) {
        a.clear(); tmp.resize(points.size());
        for (int i = 0; i < int(points.size()); ++ i) a.push_back({points[i], i});
        sort(a.begin(), a.end(), [](Node u, Node v) { return u.p < v.p; });
        ids = {-1, -1}; d2 = numeric_limits < db >::infinity();
        dfs(0, int(a.size()));
        return ids;
    }
};

/*
ClosestPair cp; auto [u, v] = cp.build({{0, 0}, {3, 4}});
if (u != -1) cout << sqrtl(cp.d2);
*/
