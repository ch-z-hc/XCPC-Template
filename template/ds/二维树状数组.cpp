/*
单点加、闭矩形求和。建树时给出所有可能修改的点，查询边界任意，和须放入 ll。
建树 O(n log² n)，修改和查询 O(log² n)。
prefix 默认统计横、纵坐标均不大于给定值的点。strict_x、strict_y 分别将对应边界改为严格小于。
*/

#include <bits/stdc++.h>

#define V vector
#define fi first

typedef long long ll;

using namespace std;

struct Fenwick2D {
    V < ll > xs;
    V < V < ll > > ys, tr;

    void build(const V < pair < ll, ll > > &points) {
        xs.clear();
        for (auto &p : points) xs.push_back(p.fi);
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int n = int(xs.size());
        ys.assign(n + 1, {}); tr.assign(n + 1, {});
        for (auto [x, y] : points) {
            int p = int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
            for (int i = p; i <= n; i += i & -i) ys[i].push_back(y);
        }
        for (int i = 1; i <= n; i ++) {
            sort(ys[i].begin(), ys[i].end());
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
            tr[i].assign(ys[i].size() + 1, 0);
        }
    }

    void add(ll x, ll y, ll v) {
        int p = int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
        for (int i = p; i <= int(xs.size()); i += i & -i) {
            int q = int(lower_bound(ys[i].begin(), ys[i].end(), y)
                        - ys[i].begin()) + 1;
            for (int j = q; j < int(tr[i].size()); j += j & -j) tr[i][j] += v;
        }
    }

    ll prefix(ll x, ll y, bool strict_x = false, bool strict_y = false) {
        auto it = strict_x ? lower_bound(xs.begin(), xs.end(), x)
                           : upper_bound(xs.begin(), xs.end(), x);
        ll ans = 0;
        for (int i = int(it - xs.begin()); i; i -= i & -i) {
            auto jt = strict_y ? lower_bound(ys[i].begin(), ys[i].end(), y)
                               : upper_bound(ys[i].begin(), ys[i].end(), y);
            for (int j = int(jt - ys[i].begin()); j; j -= j & -j) ans += tr[i][j];
        }
        return ans;
    }

    ll query(ll x1, ll y1, ll x2, ll y2) {
        if (x1 > x2 || y1 > y2) return 0;
        return prefix(x2, y2) - prefix(x1, y2, true) - prefix(x2, y1, false, true) +
               prefix(x1, y1, true, true);
    }
};

/*
Fenwick2D bit; bit.build({{1, 2}, {3, 4}}); bit.add(1, 2, 5);
ll pre = bit.prefix(3, 4);
ll strict = bit.prefix(3, 4, true, false);
ll sum = bit.query(1, 2, 3, 4);
*/
