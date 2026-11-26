/*
静态点集的最近邻和闭矩形计数。坐标绝对值 <= 10^9，点按输入顺序从 0 编号。
建树期望 O(n log n)，查询最坏 O(n)。等距取编号较小者。
nearest 可按编号排除一个点，有解时写入 {距离平方, 点编号}，无候选点返回 false。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct KDTree {
    struct Node {
        i64 x = 0, y = 0, lx = 0, ly = 0, rx = 0, ry = 0;
        int id = 0, l = -1, r = -1, siz = 1;
    };
    std::vector<Node> tr;
    int root = -1;

    int build(int l, int r, int dep) {
        if (l > r) return -1;
        int mid = (l + r) / 2;
        std::nth_element(tr.begin() + l, tr.begin() + mid, tr.begin() + r + 1,
                         [&](const Node &a, const Node &b) {
                             if (dep & 1) {
                                 if (a.y != b.y) return a.y < b.y;
                                 if (a.x != b.x) return a.x < b.x;
                             } else {
                                 if (a.x != b.x) return a.x < b.x;
                                 if (a.y != b.y) return a.y < b.y;
                             }
                             return a.id < b.id;
                         });
        tr[mid].l = build(l, mid - 1, dep + 1);
        tr[mid].r = build(mid + 1, r, dep + 1);
        Node &u = tr[mid];
        u.lx = u.rx = u.x;
        u.ly = u.ry = u.y;
        u.siz = 1;
        for (int v : {u.l, u.r})
            if (v != -1) {
                u.lx = std::min(u.lx, tr[v].lx);
                u.rx = std::max(u.rx, tr[v].rx);
                u.ly = std::min(u.ly, tr[v].ly);
                u.ry = std::max(u.ry, tr[v].ry);
                u.siz += tr[v].siz;
            }
        return mid;
    }

    void build(const std::vector<std::pair<i64, i64>> &ps) {
        tr.assign(ps.size(), {});
        for (int i = 0; i < (int)ps.size(); i ++) {
            tr[i].x = ps[i].first;
            tr[i].y = ps[i].second;
            tr[i].id = i;
        }
        root = build(0, (int)ps.size() - 1, 0);
    }

    i64 distance(int u, i64 x, i64 y) {
        i64 dx = x - std::clamp(x, tr[u].lx, tr[u].rx);
        i64 dy = y - std::clamp(y, tr[u].ly, tr[u].ry);
        return dx * dx + dy * dy;
    }

    void search(int u, i64 x, i64 y, int exclude, std::pair<i64, int> &ans) {
        if (u == -1) return;
        auto &p = tr[u];
        i64 dx = x - p.x, dy = y - p.y;
        if (p.id != exclude) ans = std::min(ans, std::pair{dx * dx + dy * dy, p.id});
        int a = p.l, b = p.r;
        i64 d1 = a == -1 ? LLONG_MAX : distance(a, x, y);
        i64 d2 = b == -1 ? LLONG_MAX : distance(b, x, y);
        if (d1 > d2) {
            std::swap(a, b);
            std::swap(d1, d2);
        }
        if (a != -1 && d1 <= ans.first) search(a, x, y, exclude, ans);
        if (b != -1 && d2 <= ans.first) search(b, x, y, exclude, ans);
    }

    bool nearest(i64 x, i64 y, std::pair<i64, int> &ans, int exclude = -1) {
        ans = {LLONG_MAX, INT_MAX};
        search(root, x, y, exclude, ans);
        return ans.second != INT_MAX;
    }

    int query(int u, i64 lx, i64 ly, i64 rx, i64 ry) {
        if (u == -1) return 0;
        auto &p = tr[u];
        if (p.rx < lx || rx < p.lx || p.ry < ly || ry < p.ly) return 0;
        if (lx <= p.lx && p.rx <= rx && ly <= p.ly && p.ry <= ry) return p.siz;
        int ans = lx <= p.x && p.x <= rx && ly <= p.y && p.y <= ry;
        return ans + query(p.l, lx, ly, rx, ry) + query(p.r, lx, ly, rx, ry);
    }

    int query(i64 lx, i64 ly, i64 rx, i64 ry) {
        if (lx > rx || ly > ry) return 0;
        return query(root, lx, ly, rx, ry);
    }
};

/*
KDTree kd; kd.build({{0, 0}, {2, 1}});
std::pair<i64, int> ans;
if (kd.nearest(0, 0, ans, 0)) cout << ans.first << ' ' << ans.second;
int cnt = kd.query(0, 0, 1, 1);
*/
