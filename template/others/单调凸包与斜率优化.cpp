#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 单调凸包优化：加入的斜率 k 不增，查询横坐标 x 不减，求最小 kx + b。
// 允许相等斜率和相等查询；查询前必须已有直线，无模数要求。
// x、k、b 和返回值均为 ll；差值、交叉乘积与 kx + b 的中间量不能溢出。
// |x|、|k|、|b| <= 10^9 是充分限制；DP 须按实际截距重新检查。
// add / query 均摊 O(1)，空间 O(插入总数)；不返回最优直线编号。
struct CHT {
    struct Line {
        ll k, b;
        ll eval(ll x) { return k * x + b; }
    };
    V < Line > q;
    int head = 0;

    void clear() { q.clear(); head = 0; }

    bool redundant(Line a, Line b, Line c) {
        return (b.b - a.b) * (b.k - c.k) >= (c.b - b.b) * (a.k - b.k);
    }

    void add(ll k, ll b) {
        Line c{k, b};
        if (int(q.size()) > head && q.back().k == k) {
            if (q.back().b <= b) return;
            q.pop_back();
        }
        while (int(q.size()) - head >= 2 &&
               redundant(q[q.size() - 2], q.back(), c)) q.pop_back();
        q.push_back(c);
    }

    ll query(ll x) {
        while (head + 1 < int(q.size()) && q[head + 1].eval(x) <= q[head].eval(x))
            ++ head;
        return q[head].eval(x);
    }
};

// 调用：CHT hull; hull.add(k, b); ll ans = hull.query(x); cout << ans << '\n';
