/*
按斜率不增加线，按 x 不减查询最小值，两种操作均摊 O(1)。先加线再查，允许相同斜率和查询位置。
差值、叉积和 kx + b 须放入 i64；|k|、|b|、|x| 均 <= 10^9 是充分条件。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct CHT {
    struct Line {
        i64 k, b;
        i64 eval(i64 x) { return k * x + b; }
    };
    std::vector<Line> q;
    int head = 0;

    void clear() {
        q.clear();
        head = 0;
    }

    bool redundant(Line a, Line b, Line c) {
        return (b.b - a.b) * (b.k - c.k) >= (c.b - b.b) * (a.k - b.k);
    }

    void add(i64 k, i64 b) {
        Line c{k, b};
        if (int(q.size()) > head && q.back().k == k) {
            if (q.back().b <= b) return;
            q.pop_back();
        }
        while (int(q.size()) - head >= 2 && redundant(q[q.size() - 2], q.back(), c))
            q.pop_back();
        q.push_back(c);
    }

    i64 query(i64 x) {
        while (head + 1 < int(q.size()) && q[head + 1].eval(x) <= q[head].eval(x))
            ++ head;
        return q[head].eval(x);
    }
};

/*
CHT hull; hull.add(3, 1); hull.add(1, 2);
i64 val = hull.query(4); hull.clear();
*/
