#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef __int128_t CHTInt;

using namespace std;

// 单调凸包优化：加入的斜率 k 不增，查询横坐标 x 不减，求最小 kx + b。
// 允许相等斜率和相等查询；查询前必须已有直线，无模数要求。
// x、k 为 ll，截距与返回值为 __int128，需 GCC / Clang。
// 交叉乘积与 kx + b 均须放入 __int128；|k|、|b| <= 10^18 是充分限制。
// add / query 均摊 O(1)，空间 O(插入总数)；不返回最优直线编号。
struct CHT {
    struct Line {
        ll k;
        CHTInt b;
        CHTInt eval(ll x) const { return CHTInt(k) * x + b; }
    };
    V < Line > q;
    int head = 0;

    void clear() { q.clear(); head = 0; }

    bool redundant(Line a, Line b, Line c) const {
        return (CHTInt(b.b) - a.b) * (CHTInt(b.k) - c.k) >=
               (CHTInt(c.b) - b.b) * (CHTInt(a.k) - b.k);
    }

    void add(ll k, CHTInt b) {
        Line c{k, b};
        if (int(q.size()) > head && q.back().k == k) {
            if (q.back().b <= b) return;
            q.pop_back();
        }
        while (int(q.size()) - head >= 2 &&
               redundant(q[q.size() - 2], q.back(), c)) q.pop_back();
        q.push_back(c);
    }

    CHTInt query(ll x) {
        while (head + 1 < int(q.size()) && q[head + 1].eval(x) <= q[head].eval(x))
            ++ head;
        return q[head].eval(x);
    }
};

// 调用：CHT hull; hull.add(k, b); CHTInt ans = hull.query(x);
// 输出先复制开头 template 的 print128：print128(ans); cout << '\n';
