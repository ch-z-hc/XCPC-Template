#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 树状数组：单点加、前缀和、区间和；下标为 1-based，区间为闭区间。
// 初始化 O(n)，修改和查询 O(log n)，空间 O(n)。
// 要求 add 的下标至少为 1，计算不溢出 ll；init 可重新初始化。
struct Fenwick {
    int n = 0;
    V < ll > tr;

    Fenwick(int n = 0) { init(n); }
    Fenwick(const V < ll > &a) { init(a); }

    void init(int n_) {
        n = n_;
        tr.assign(n + 1, 0);
    }

    void init(const V < ll > &a) {
        if (&a == &tr) {
            V < ll > b = a;
            init(b); return;
        }
        init(max(0, int(a.size()) - 1));
        for (int i = 1; i <= n; ++ i) {
            tr[i] += a[i];
            int p = i + (i & -i);
            if (p <= n) tr[p] += tr[i];
        }
    }

    void add(int x, ll v) {
        for (; x <= n; x += x & -x) tr[x] += v;
    }

    ll query(int x) {
        ll ans = 0;
        for (; x; x -= x & -x) ans += tr[x];
        return ans;
    }

    ll query(int l, int r) {
        return l <= r ? query(r) - query(l - 1) : 0;
    }

    // 原数组各项非负，返回前缀和 >= k 的最小位置；不存在返回 n + 1。
    // k <= 0 返回 1；时间 O(log n)，不需要二分套 query。
    int kth(ll k) {
        int x = 0, step = 1;
        while (step <= n / 2) step <<= 1;
        for (; step; step >>= 1) {
            int y = x + step;
            if (y <= n && tr[y] < k) x = y, k -= tr[y];
        }
        return x + 1;
    }
};

// 用法：Fenwick bit(a); // a[0] 为占位。
// bit.add(x, v);
// ll ans = bit.query(l, r);
