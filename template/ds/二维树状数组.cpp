/*
单点加、闭矩形求和。建树时给出所有可能修改的点，查询边界任意，和须放入 i64。
建树 O(n log² n)，修改和查询 O(log² n)。
prefix 默认统计横、纵坐标均不大于给定值的点。strict_x、strict_y 分别将对应边界改为严格小于。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct Fenwick2D {
    std::vector<i64> xs;
    std::vector<std::vector<i64>> ys, tr;

    void build(const std::vector<std::pair<i64, i64>> &points) {
        xs.clear();
        for (auto &p : points) xs.push_back(p.first);
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        int n = int(xs.size());
        ys.assign(n + 1, {});
        tr.assign(n + 1, {});
        for (auto [x, y] : points) {
            int p = int(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
            for (int i = p; i <= n; i += i & -i) ys[i].push_back(y);
        }
        for (int i = 1; i <= n; i ++) {
            std::sort(ys[i].begin(), ys[i].end());
            ys[i].erase(std::unique(ys[i].begin(), ys[i].end()), ys[i].end());
            tr[i].assign(ys[i].size() + 1, 0);
        }
    }

    void add(i64 x, i64 y, i64 v) {
        int p = int(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
        for (int i = p; i <= int(xs.size()); i += i & -i) {
            int q = int(std::lower_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin()) + 1;
            for (int j = q; j < int(tr[i].size()); j += j & -j) tr[i][j] += v;
        }
    }

    i64 prefix(i64 x, i64 y, bool strict_x = false, bool strict_y = false) {
        auto it = strict_x ? std::lower_bound(xs.begin(), xs.end(), x)
                           : std::upper_bound(xs.begin(), xs.end(), x);
        i64 ans = 0;
        for (int i = int(it - xs.begin()); i; i -= i & -i) {
            auto jt = strict_y ? std::lower_bound(ys[i].begin(), ys[i].end(), y)
                               : std::upper_bound(ys[i].begin(), ys[i].end(), y);
            for (int j = int(jt - ys[i].begin()); j; j -= j & -j) ans += tr[i][j];
        }
        return ans;
    }

    i64 query(i64 x1, i64 y1, i64 x2, i64 y2) {
        if (x1 > x2 || y1 > y2) return 0;
        return prefix(x2, y2) - prefix(x1, y2, true) - prefix(x2, y1, false, true) +
               prefix(x1, y1, true, true);
    }
};

/*
Fenwick2D bit; bit.build({{1, 2}, {3, 4}}); bit.add(1, 2, 5);
i64 pre = bit.prefix(3, 4);
i64 strict = bit.prefix(3, 4, true, false);
i64 sum = bit.query(1, 2, 3, 4);
*/
