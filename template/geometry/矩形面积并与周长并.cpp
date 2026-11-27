/*
轴对齐矩形的面积并、周长并，O(n log n)。坐标可负，允许重复和退化矩形。
坐标差、面积和周长的中间量须放入 ll。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct RectUnion {
    struct Event {
        ll x, y1, y2;
        int d;
    };
    struct Node {
        int cover = 0, cnt = 0;
        ll len = 0;
        bool left = 0, right = 0;
    };
    V < ll > ys;
    V < Node > tr;

    void pull(int p, int l, int r) {
        if (tr[p].cover) {
            tr[p].len = ys[r + 1] - ys[l];
            tr[p].cnt = 1;
            tr[p].left = tr[p].right = true;
        } else if (l == r) {
            tr[p].len = tr[p].cnt = 0;
            tr[p].left = tr[p].right = false;
        } else {
            auto a = tr[p * 2], b = tr[p * 2 + 1];
            tr[p].len = a.len + b.len;
            tr[p].cnt = a.cnt + b.cnt - (a.right && b.left);
            tr[p].left = a.left;
            tr[p].right = b.right;
        }
    }
    void modify(int p, int l, int r, int x, int y, int d) {
        if (x <= l && r <= y)
            tr[p].cover += d;
        else {
            int mid = (l + r) / 2;
            if (x <= mid) modify(p * 2, l, mid, x, y, d);
            if (y > mid) modify(p * 2 + 1, mid + 1, r, x, y, d);
        }
        pull(p, l, r);
    }
    pair < ll, ll > solve(const V < array < ll, 4 > > &rects) {
        V < Event > events;
        ys.clear();
        for (auto [x1, y1, x2, y2] : rects) {
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            if (x1 == x2 || y1 == y2) continue;
            events.push_back({x1, y1, y2, 1});
            events.push_back({x2, y1, y2, -1});
            ys.push_back(y1);
            ys.push_back(y2);
        }
        if (events.empty()) return {0, 0};
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        sort(events.begin(), events.end(),
                  [](Event a, Event b) { return a.x != b.x ? a.x < b.x : a.d > b.d; });
        int n = int(ys.size()) - 1;
        tr.assign(n * 4 + 5, {});
        ll area = 0, perimeter = 0, last = events[0].x;
        for (auto [x, y1, y2, d] : events) {
            area += tr[1].len * (x - last);
            perimeter += 2ll * tr[1].cnt * (x - last);
            ll before = tr[1].len;
            int l = int(lower_bound(ys.begin(), ys.end(), y1) - ys.begin());
            int r = int(lower_bound(ys.begin(), ys.end(), y2) - ys.begin()) - 1;
            modify(1, 0, n - 1, l, r, d);
            perimeter += abs(tr[1].len - before);
            last = x;
        }
        return {area, perimeter};
    }
};

// RectUnion scan; auto [area, perimeter] = scan.solve({{0, 0, 2, 3}});
