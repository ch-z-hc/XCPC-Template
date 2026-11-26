// 下标从 1 开始，add 的位置须在 [1, n] 内。init(n) 重置为全 0。

#include <bits/stdc++.h>

using i64 = long long;

struct Fenwick {
    int n = 0;
    std::vector<i64> tr;

    Fenwick(int n = 0) { init(n); }
    Fenwick(const std::vector<i64> &a) { init(a); }

    void init(int n_) {
        n = n_;
        tr.assign(n + 1, 0);
    }

    void init(const std::vector<i64> &a) {
        if (&a == &tr) {
            std::vector<i64> b = a;
            init(b);
            return;
        }
        init(std::max(0, int(a.size()) - 1));
        for (int i = 1; i <= n; i ++) {
            tr[i] += a[i];
            int p = i + (i & -i);
            if (p <= n) tr[p] += tr[i];
        }
    }

    void add(int x, i64 v) {
        for (; x <= n; x += x & -x) tr[x] += v;
    }

    i64 query(int x) {
        i64 ans = 0;
        for (; x; x -= x & -x) ans += tr[x];
        return ans;
    }

    i64 query(int l, int r) { return l <= r ? query(r) - query(l - 1) : 0; }
};

/*
Fenwick bit; bit.init({0, 2, 5}); bit.add(1, 3);
i64 pre = bit.query(1), sum = bit.query(1, 2);
bit.init(2);
*/
