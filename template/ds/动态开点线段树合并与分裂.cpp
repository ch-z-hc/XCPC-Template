/*
维护值域 [1, n] 的多重集，根 0 表示空集。计数非负，总数须放入 ll。
合并的两棵树不能共用节点。合并耗时与重叠节点数成正比，其余操作 O(log n)，节点不回收。
split 从原树取走指定区间并返回新根，merge(a, b) 将 b 并入 a 后把 b 置零。
*/

#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
#define pii pair < int, int >
typedef long long ll;
using namespace std;

struct MergeSegTree {
    struct Node {
        int ls = 0, rs = 0;
        ll sum = 0;
    };
    int n;
    V < Node > tr;

    MergeSegTree(int n = 1) { init(n); }
    void init(int n_) {
        n = n_;
        tr.assign(1, {});
    }
    int node() {
        tr.push_back({});
        return int(tr.size()) - 1;
    }
    void pull(int p) { tr[p].sum = tr[tr[p].ls].sum + tr[tr[p].rs].sum; }

    int add(int p, int l, int r, int x, ll v) {
        if (!p) p = node();
        if (l == r) {
            tr[p].sum += v;
            return p;
        }
        int mid = l + (r - l) / 2;
        if (x <= mid) tr[p].ls = add(tr[p].ls, l, mid, x, v);
        else tr[p].rs = add(tr[p].rs, mid + 1, r, x, v);
        pull(p);
        return p;
    }

    void add(int &root, int x, ll v) { root = add(root, 1, n, x, v); }

    ll query(int p, int l, int r, int x, int y) {
        if (!p || y < l || r < x) return 0;
        if (x <= l && r <= y) return tr[p].sum;
        int mid = l + (r - l) / 2;
        return query(tr[p].ls, l, mid, x, y) + query(tr[p].rs, mid + 1, r, x, y);
    }

    ll query(int root, int l, int r) { return query(root, 1, n, l, r); }

    int kth(int p, int l, int r, ll k) {
        if (l == r) return l;
        int mid = l + (r - l) / 2;
        ll cnt = tr[tr[p].ls].sum;
        if (k <= cnt) return kth(tr[p].ls, l, mid, k);
        return kth(tr[p].rs, mid + 1, r, k - cnt);
    }

    int kth(int root, ll k) {
        return k >= 1 && k <= tr[root].sum ? kth(root, 1, n, k) : -1;
    }

    int merge(int u, int v, int l, int r) {
        if (!u) return v;
        if (!v) return u;
        if (l == r) {
            tr[u].sum += tr[v].sum;
            return u;
        }
        int mid = l + (r - l) / 2;
        tr[u].ls = merge(tr[u].ls, tr[v].ls, l, mid);
        tr[u].rs = merge(tr[u].rs, tr[v].rs, mid + 1, r);
        pull(u);
        return u;
    }

    void merge(int &u, int &v) {
        u = merge(u, v, 1, n); v = 0;
    }

    pii split(int p, int l, int r, int x, int y) {
        if (!p || y < l || r < x) return {p, 0};
        if (x <= l && r <= y) return {0, p};
        int q = node(), mid = l + (r - l) / 2;
        auto a = split(tr[p].ls, l, mid, x, y);
        auto b = split(tr[p].rs, mid + 1, r, x, y);
        tr[p].ls = a.fi; tr[q].ls = a.se;
        tr[p].rs = b.fi; tr[q].rs = b.se;
        pull(p); pull(q);
        return {p, q};
    }

    int split(int &root, int l, int r) {
        auto [a, b] = split(root, 1, n, l, r);
        root = a;
        return b;
    }
};

/*
MergeSegTree st; st.init(10); int a = 0;
st.add(a, 3, 2); st.add(a, 6, 1);
int b = st.split(a, 2, 4);
ll cnt = st.query(b, 1, 5); int x = st.kth(b, 2);
st.merge(a, b);
*/
