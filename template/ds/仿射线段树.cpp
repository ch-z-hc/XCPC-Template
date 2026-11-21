#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
对 a[l..r] 做 a[i] = a[i] * mul + add，查询区间和。数组从 1 开始，结果对 mod 取模。
参数可为负，1 <= mod <= INT_MAX。整段赋值为 v 时传 mul = 0、add = v。
建树 O(n)，修改和查询 O(log n)，空间 O(n)。
*/
struct AffineSeg {
    int n = 0;
    ll mod = 1;
    V < ll > sum, mul, add;

    ll norm(ll x) { x %= mod; return x < 0 ? x + mod : x; }
    void pull(int p) { sum[p] = (sum[p << 1] + sum[p << 1 | 1]) % mod; }

    void apply(int p, int l, int r, ll a, ll b) {
        sum[p] = (sum[p] * a + (r - l + 1ll) * b) % mod;
        mul[p] = mul[p] * a % mod;
        add[p] = (add[p] * a + b) % mod;
    }

    void push(int p, int l, int r) {
        int mid = (l + r) >> 1;
        apply(p << 1, l, mid, mul[p], add[p]);
        apply(p << 1 | 1, mid + 1, r, mul[p], add[p]);
        mul[p] = 1 % mod; add[p] = 0;
    }

    void build(int p, int l, int r, const V < ll > &a) {
        if (l == r) { sum[p] = norm(a[l]); return; }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a); build(p << 1 | 1, mid + 1, r, a);
        pull(p);
    }

    void init(const V < ll > &a, ll mod_) {
        if (&a == &sum || &a == &mul || &a == &add) {
            V < ll > b = a;
            init(b, mod_); return;
        }
        mod = mod_; n = max(0, int(a.size()) - 1);
        sum.assign(4 * n + 5, 0); add = sum;
        mul.assign(4 * n + 5, 1 % mod);
        if (n) build(1, 1, n, a);
    }

    void modify(int p, int l, int r, int x, int y, ll a, ll b) {
        if (x <= l && r <= y) { apply(p, l, r, a, b); return; }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (x <= mid) modify(p << 1, l, mid, x, y, a, b);
        if (y > mid) modify(p << 1 | 1, mid + 1, r, x, y, a, b);
        pull(p);
    }

    void modify(int l, int r, ll a, ll b) {
        if (n && l <= r) modify(1, 1, n, l, r, norm(a), norm(b));
    }

    ll query(int p, int l, int r, int x, int y) {
        if (x <= l && r <= y) return sum[p];
        push(p, l, r);
        int mid = (l + r) >> 1;
        ll ans = 0;
        if (x <= mid) ans += query(p << 1, l, mid, x, y);
        if (y > mid) ans += query(p << 1 | 1, mid + 1, r, x, y);
        return ans % mod;
    }

    ll query(int l, int r) { return n && l <= r ? query(1, 1, n, l, r) : 0; }
};

/*
AffineSeg st; st.init(a, mod); st.modify(l, r, mul, add);
ll ans = st.query(l, r);
*/
