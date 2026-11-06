#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// CDQ 三维偏序，输入为 0-based 点集，可重复，坐标为 ll。
// 返回每个点被多少个其他点支配：三个坐标均 <= 当前点的坐标。
// 重复点互相计数；时间 O(n log^2 n)，空间 O(n)，不修改输入。

V < ll > dominance(const V < array < ll, 3 > > &a) {
    struct Node { ll x, y, z; int cnt, id, p; ll ans; };
    int n = int(a.size());
    V < int > ord(n), bel(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int u, int v) { return a[u] < a[v]; });
    V < Node > b;
    V < ll > val;
    for (int i : ord) {
        if (b.empty() || array < ll, 3 >{b.back().x, b.back().y, b.back().z} != a[i])
            b.push_back({a[i][0], a[i][1], a[i][2], 0, int(b.size()), 0, 0});
        b.back().cnt ++; bel[i] = b.back().id;
        val.push_back(a[i][2]);
    }
    sort(val.begin(), val.end());
    val.erase(unique(val.begin(), val.end()), val.end());
    for (auto &p : b)
        p.p = int(lower_bound(val.begin(), val.end(), p.z) - val.begin()) + 1;
    V < int > bit(val.size() + 1);
    auto add = [&](int p, int x) {
        for (; p < int(bit.size()); p += p & -p) bit[p] += x;
    };
    auto sum = [&](int p) {
        int ans = 0;
        for (; p; p -= p & -p) ans += bit[p];
        return ans;
    };
    V < Node > tmp(b.size());
    auto cdq = [&](auto &&self, int l, int r) -> void {
        if (l >= r) return;
        int mid = (l + r) >> 1;
        self(self, l, mid); self(self, mid + 1, r);
        int i = l, j = mid + 1, k = l;
        while (j <= r) {
            while (i <= mid && b[i].y <= b[j].y) {
                add(b[i].p, b[i].cnt);
                tmp[k ++] = b[i ++];
            }
            b[j].ans += sum(b[j].p);
            tmp[k ++] = b[j ++];
        }
        for (int p = l; p < i; ++ p) add(b[p].p, -b[p].cnt);
        while (i <= mid) tmp[k ++] = b[i ++];
        for (int p = l; p <= r; ++ p) b[p] = tmp[p];
    };
    cdq(cdq, 0, int(b.size()) - 1);
    V < ll > ans(n), cnt(b.size());
    for (auto &p : b) cnt[p.id] = p.ans + p.cnt - 1;
    for (int i = 0; i < n; ++ i) ans[i] = cnt[bel[i]];
    return ans;
}

// 用法：auto cnt = dominance(points);
