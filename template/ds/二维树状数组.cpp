#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 离线建坐标、在线单点加与矩形和；坐标为任意 ll，重复点允许。
// build(points) 必须包含此后所有 add 的坐标，query 的坐标不受限制。
// 预处理 O(n log^2 n)，操作 O(log^2 n)，空间 O(n log n)，累加不溢出 ll。
struct Fenwick2D {
    V < ll > xs;
    V < V < ll > > ys, tr;

    void build(const V < pair < ll, ll > > &points) {
        xs.clear();
        for (auto [x, y] : points) { (void)y; xs.push_back(x); }
        sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int n = int(xs.size());
        ys.assign(n + 1, {}); tr.assign(n + 1, {});
        for (auto [x, y] : points) {
            int p = int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
            for (int i = p; i <= n; i += i & -i) ys[i].push_back(y);
        }
        for (int i = 1; i <= n; ++ i) {
            sort(ys[i].begin(), ys[i].end());
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
            tr[i].assign(ys[i].size() + 1, 0);
        }
    }

    void add(ll x, ll y, ll v) {
        int p = int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
        for (int i = p; i <= int(xs.size()); i += i & -i) {
            int q = int(lower_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin()) + 1;
            for (int j = q; j < int(tr[i].size()); j += j & -j) tr[i][j] += v;
        }
    }

    // 前缀矩形：strict_x / strict_y 为真时对应方向使用严格小于。
    ll prefix(ll x, ll y, bool strict_x = false, bool strict_y = false) const {
        auto it = strict_x ? lower_bound(xs.begin(), xs.end(), x) :
                            upper_bound(xs.begin(), xs.end(), x);
        ll ans = 0;
        for (int i = int(it - xs.begin()); i; i -= i & -i) {
            auto jt = strict_y ? lower_bound(ys[i].begin(), ys[i].end(), y) :
                                 upper_bound(ys[i].begin(), ys[i].end(), y);
            for (int j = int(jt - ys[i].begin()); j; j -= j & -j) ans += tr[i][j];
        }
        return ans;
    }

    ll query(ll x1, ll y1, ll x2, ll y2) const {
        if (x1 > x2 || y1 > y2) return 0;
        return prefix(x2, y2) - prefix(x1, y2, true) -
               prefix(x2, y1, false, true) + prefix(x1, y1, true, true);
    }
};

// Fenwick2D bit; bit.build(points); bit.add(x, y, v);
// ll ans = bit.query(x1, y1, x2, y2); // 闭矩形。
