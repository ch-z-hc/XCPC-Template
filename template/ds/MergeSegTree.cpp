#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 动态开点权值线段树，值域 [1, n]，1 <= n <= INT_MAX；根编号 0 表示空树。
// add / query / kth / split 为 O(log n)；merge 与重叠节点数成正比。
// 总空间 O(开点数)，不回收节点；计数始终非负，所有 sum 不溢出 ll。
// merge / split 会消耗原树，不可持久化，也不能让两个根共享非空节点。
struct MergeSegTree {
    struct Node { int ls = 0, rs = 0; ll sum = 0; };
    int n;
    V < Node > tr;

    MergeSegTree(int n = 1) { init(n); }
    void init(int n_) { n = n_; tr.assign(1, {}); }
    int node() { tr.push_back({}); return int(tr.size()) - 1; }
    void pull(int p) { tr[p].sum = tr[tr[p].ls].sum + tr[tr[p].rs].sum; }

    int add(int p, int l, int r, int x, ll v) {
        if (! p) p = node();
        if (l == r) { tr[p].sum += v; return p; }
        int mid = l + (r - l) / 2;
        if (x <= mid) tr[p].ls = add(tr[p].ls, l, mid, x, v);
        else tr[p].rs = add(tr[p].rs, mid + 1, r, x, v);
        pull(p);
        return p;
    }

    void add(int &root, int x, ll v) { root = add(root, 1, n, x, v); }

    ll query(int p, int l, int r, int x, int y) const {
        if (! p || y < l || r < x) return 0;
        if (x <= l && r <= y) return tr[p].sum;
        int mid = l + (r - l) / 2;
        return query(tr[p].ls, l, mid, x, y) + query(tr[p].rs, mid + 1, r, x, y);
    }

    ll query(int root, int l, int r) const { return query(root, 1, n, l, r); }

    int kth(int p, int l, int r, ll k) const {
        if (l == r) return l;
        int mid = l + (r - l) / 2;
        ll cnt = tr[tr[p].ls].sum;
        if (k <= cnt) return kth(tr[p].ls, l, mid, k);
        return kth(tr[p].rs, mid + 1, r, k - cnt);
    }

    int kth(int root, ll k) const {
        return k >= 1 && k <= tr[root].sum ? kth(root, 1, n, k) : -1;
    }

    int merge(int u, int v, int l, int r) {
        if (! u || ! v) return u | v;
        if (l == r) { tr[u].sum += tr[v].sum; return u; }
        int mid = l + (r - l) / 2;
        tr[u].ls = merge(tr[u].ls, tr[v].ls, l, mid);
        tr[u].rs = merge(tr[u].rs, tr[v].rs, mid + 1, r);
        pull(u);
        return u;
    }

    void merge(int &u, int &v) { u = merge(u, v, 1, n); v = 0; }

    pair < int, int > split(int p, int l, int r, int x, int y) {
        if (! p || y < l || r < x) return {p, 0};
        if (x <= l && r <= y) return {0, p};
        int q = node(), mid = l + (r - l) / 2;
        auto a = split(tr[p].ls, l, mid, x, y);
        auto b = split(tr[p].rs, mid + 1, r, x, y);
        tr[p].ls = a.first; tr[q].ls = a.second;
        tr[p].rs = b.first; tr[q].rs = b.second;
        pull(p); pull(q);
        return {p, q};
    }

    int split(int &root, int l, int r) {
        auto [a, b] = split(root, 1, n, l, r);
        root = a;
        return b;
    }
};

// MergeSegTree st(n); int a = 0; st.add(a, x, cnt);
// int b = st.split(a, l, r); st.merge(a, b); ll sum = st.query(a, l, r);
