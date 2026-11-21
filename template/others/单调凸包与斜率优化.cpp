#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
按斜率不增的顺序加入直线 kx + b，按 x 不减的顺序查询最小值。可以取等号，先加线再查。
差值、叉积和 kx + b 用 ll。|k|、|b|、|x| 都不超过 10^9 时安全，做 DP 时也要检查截距。
增线、查询均摊 O(1)，空间 O(插入总数)。
*/
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

/*
CHT hull; hull.add(k, b);
ll ans = hull.query(x);
*/
