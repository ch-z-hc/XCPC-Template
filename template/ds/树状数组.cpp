// 下标从 1 开始，add 的位置须在 [1, n] 内。init(n) 重置为全 0。

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

struct Fenwick {
    int n = 0;
    V < ll > tr;

    Fenwick(int n = 0) { init(n); }
    Fenwick(const V < ll > &a) { init(a); }

    void init(int n_) {
        n = n_; tr.assign(n + 1, 0);
    }

    void init(const V < ll > &a) {
        if (&a == &tr) {
            V < ll > b = a;
            init(b);
            return;
        }
        init(max(0, int(a.size()) - 1));
        for (int i = 1; i <= n; i ++) {
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

    ll query(int l, int r) { return l <= r ? query(r) - query(l - 1) : 0; }
};

/*
Fenwick bit; bit.init({0, 2, 5}); bit.add(1, 3);
ll pre = bit.query(1), sum = bit.query(1, 2);
bit.init(2);
*/
