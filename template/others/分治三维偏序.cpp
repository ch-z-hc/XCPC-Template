// 统计每个点以外三个坐标均不大于它的点数，重合点互相计数。结果按输入顺序，O(n log² n)。

#include <bits/stdc++.h>

using i64 = long long;

std::vector<i64> dominance(const std::vector<std::array<i64, 3>> &a) {
    struct Node {
        i64 x, y, z;
        int cnt, id, p;
        i64 ans;
    };
    int n = int(a.size());
    std::vector<int> ord(n), bel(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::sort(ord.begin(), ord.end(), [&](int u, int v) { return a[u] < a[v]; });
    std::vector<Node> b;
    std::vector<i64> val;
    for (int i : ord) {
        if (b.empty() || std::array<i64, 3>{b.back().x, b.back().y, b.back().z} != a[i])
            b.push_back({a[i][0], a[i][1], a[i][2], 0, int(b.size()), 0, 0});
        b.back().cnt ++;
        bel[i] = b.back().id;
        val.push_back(a[i][2]);
    }
    std::sort(val.begin(), val.end());
    val.erase(std::unique(val.begin(), val.end()), val.end());
    for (auto &p : b)
        p.p = int(std::lower_bound(val.begin(), val.end(), p.z) - val.begin()) + 1;
    std::vector<int> bit(val.size() + 1);
    auto add = [&](int p, int x) {
        for (; p < int(bit.size()); p += p & -p) bit[p] += x;
    };
    auto sum = [&](int p) {
        int ans = 0;
        for (; p; p -= p & -p) ans += bit[p];
        return ans;
    };
    std::vector<Node> tmp(b.size());
    auto cdq = [&](auto &&self, int l, int r) -> void {
        if (l >= r) return;
        int mid = (l + r) >> 1;
        self(self, l, mid);
        self(self, mid + 1, r);
        int i = l, j = mid + 1, k = l;
        while (j <= r) {
            while (i <= mid && b[i].y <= b[j].y) {
                add(b[i].p, b[i].cnt);
                tmp[k ++] = b[i ++];
            }
            b[j].ans += sum(b[j].p);
            tmp[k ++] = b[j ++];
        }
        for (int p = l; p < i; p ++) add(b[p].p, -b[p].cnt);
        while (i <= mid) tmp[k ++] = b[i ++];
        for (int p = l; p <= r; p ++) b[p] = tmp[p];
    };
    cdq(cdq, 0, int(b.size()) - 1);
    std::vector<i64> ans(n), cnt(b.size());
    for (auto &p : b) cnt[p.id] = p.ans + p.cnt - 1;
    for (int i = 0; i < n; i ++) ans[i] = cnt[bel[i]];
    return ans;
}

// auto cnt = dominance(points);
