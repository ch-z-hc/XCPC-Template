/*
区间仿射变换和区间和，a[1..n]，1 <= mod <= INT_MAX，参数可负。
建树 O(n)，修改和查询 O(log n)。
modify(l, r, k, b) 将区间内每项 x 改为 kx + b，区间赋值 v 时传 0, v。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct AffineSeg {
    int n = 0;
    i64 mod = 1;
    std::vector<i64> sum, mul, add;

    i64 norm(i64 x) {
        x %= mod;
        return x < 0 ? x + mod : x;
    }
    void pull(int p) { sum[p] = (sum[p * 2] + sum[p * 2 + 1]) % mod; }

    void apply(int p, int l, int r, i64 a, i64 b) {
        sum[p] = (sum[p] * a + (r - l + 1ll) * b) % mod;
        mul[p] = mul[p] * a % mod;
        add[p] = (add[p] * a + b) % mod;
    }

    void push(int p, int l, int r) {
        int mid = (l + r) / 2;
        apply(p * 2, l, mid, mul[p], add[p]);
        apply(p * 2 + 1, mid + 1, r, mul[p], add[p]);
        mul[p] = 1 % mod;
        add[p] = 0;
    }

    void build(int p, int l, int r, const std::vector<i64> &a) {
        if (l == r) {
            sum[p] = norm(a[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(p * 2, l, mid, a);
        build(p * 2 + 1, mid + 1, r, a);
        pull(p);
    }

    void init(const std::vector<i64> &a, i64 mod_) {
        if (&a == &sum || &a == &mul || &a == &add) {
            std::vector<i64> b = a;
            init(b, mod_);
            return;
        }
        mod = mod_;
        n = std::max(0, int(a.size()) - 1);
        sum.assign(n * 4 + 5, 0);
        add = sum;
        mul.assign(n * 4 + 5, 1 % mod);
        if (n) build(1, 1, n, a);
    }

    void modify(int p, int l, int r, int x, int y, i64 a, i64 b) {
        if (x <= l && r <= y) {
            apply(p, l, r, a, b);
            return;
        }
        push(p, l, r);
        int mid = (l + r) / 2;
        if (x <= mid) modify(p * 2, l, mid, x, y, a, b);
        if (y > mid) modify(p * 2 + 1, mid + 1, r, x, y, a, b);
        pull(p);
    }

    void modify(int l, int r, i64 a, i64 b) {
        if (n && l <= r) modify(1, 1, n, l, r, norm(a), norm(b));
    }

    i64 query(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return sum[p];
        push(p, l, r);
        int mid = (l + r) / 2;
        i64 ans = 0;
        if (x <= mid) ans += query(p * 2, l, mid, x, y);
        if (y > mid) ans += query(p * 2 + 1, mid + 1, r, x, y);
        return ans % mod;
    }

    i64 query(int l, int r) { return n && l <= r ? query(1, 1, n, l, r) : 0; }
};

/*
AffineSeg st; st.init({0, 2, 3}, 17);
st.modify(1, 2, 2, 1);
i64 sum = st.query(1, 2);
*/
