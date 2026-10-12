#include <bits/stdc++.h>

#define V vector

using namespace std;
typedef long long ll;

// 静态二维 KD-tree，无模数；点和查询坐标绝对值均不超过 1e9。
// 平方距离最多 8e18，使用 ll；不支持插入、删除或点权。
// build(ps) 接收 {x, y}，原下标 0-based，重复点分别保留，可重复 build。
// nearest(x, y, ans, exclude = -1) 成功时将 {平方距离, 原下标} 写入 ans；
// 距离相等取较小下标，默认可返回自身，exclude 只排除指定原下标。
// 成功返回 true，空集或排除后为空返回 false。
// query(lx, ly, rx, ry) 统计闭矩形内点数，非法矩形返回 0。
// 建树平均 O(n log n)，空间 O(n)，递归深度 O(log n)。
// 两种查询最坏均 O(n)，最近邻不保证 O(log n)。

struct KDTree {
    struct Node {
        ll x = 0, y = 0, lx = 0, ly = 0, rx = 0, ry = 0;
        int id = 0, l = -1, r = -1, siz = 1;
    };
    V < Node > tr;
    int root = -1;

    int build(int l, int r, int dep) {
        if (l > r) return -1;
        int mid = (l + r) / 2;
        nth_element(tr.begin() + l, tr.begin() + mid, tr.begin() + r + 1,
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
        u.lx = u.rx = u.x; u.ly = u.ry = u.y; u.siz = 1;
        for (int v : {u.l, u.r}) if (v != -1) {
            u.lx = min(u.lx, tr[v].lx); u.rx = max(u.rx, tr[v].rx);
            u.ly = min(u.ly, tr[v].ly); u.ry = max(u.ry, tr[v].ry);
            u.siz += tr[v].siz;
        }
        return mid;
    }

    void build(const V < pair < ll, ll > > &ps) {
        tr.assign(ps.size(), {});
        for (int i = 0; i < (int)ps.size(); ++ i) {
            tr[i].x = ps[i].first; tr[i].y = ps[i].second; tr[i].id = i;
        }
        root = build(0, (int)ps.size() - 1, 0);
    }

    ll distance(int u, ll x, ll y) {
        ll dx = x - clamp(x, tr[u].lx, tr[u].rx);
        ll dy = y - clamp(y, tr[u].ly, tr[u].ry);
        return dx * dx + dy * dy;
    }

    void search(int u, ll x, ll y, int exclude,
                pair < ll, int > &ans) {
        if (u == -1) return;
        auto &p = tr[u];
        ll dx = x - p.x, dy = y - p.y;
        if (p.id != exclude) ans = min(ans, pair{dx * dx + dy * dy, p.id});
        int a = p.l, b = p.r;
        ll d1 = a == -1 ? LLONG_MAX : distance(a, x, y);
        ll d2 = b == -1 ? LLONG_MAX : distance(b, x, y);
        if (d1 > d2) { swap(a, b); swap(d1, d2); }
        if (a != -1 && d1 <= ans.first) search(a, x, y, exclude, ans);
        if (b != -1 && d2 <= ans.first) search(b, x, y, exclude, ans);
    }

    bool nearest(ll x, ll y, pair < ll, int > &ans, int exclude = -1) {
        ans = {LLONG_MAX, INT_MAX};
        search(root, x, y, exclude, ans);
        return ans.second != INT_MAX;
    }

    int query(int u, ll lx, ll ly, ll rx, ll ry) {
        if (u == -1) return 0;
        auto &p = tr[u];
        if (p.rx < lx || rx < p.lx || p.ry < ly || ry < p.ly) return 0;
        if (lx <= p.lx && p.rx <= rx && ly <= p.ly && p.ry <= ry)
            return p.siz;
        int ans = lx <= p.x && p.x <= rx && ly <= p.y && p.y <= ry;
        return ans + query(p.l, lx, ly, rx, ry) + query(p.r, lx, ly, rx, ry);
    }

    int query(ll lx, ll ly, ll rx, ll ry) {
        if (lx > rx || ly > ry) return 0;
        return query(root, lx, ly, rx, ry);
    }
};

// KDTree kd; kd.build(ps); pair < ll, int > ans;
// bool ok = kd.nearest(x, y, ans); // 成功时使用 ans。
// int cnt = kd.query(lx, ly, rx, ry);
