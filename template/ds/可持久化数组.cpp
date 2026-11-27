/*
支持在任意旧版本上单点赋值和查询，数组从 1 编号。初始化会清空版本，空数组不可读写。
建树 O(n)，修改、查询 O(log n)，每次修改新增 O(log n) 个节点。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct PArray {
    struct Node {
        int l = 0, r = 0;
        ll val = 0;
    };
    int n = 0, root = 0;
    V < Node > t;

    int build(int l, int r, const V < ll > &a) {
        int p = int(t.size());
        t.push_back(Node());
        if (l == r) {
            t[p].val = a[l];
            return p;
        }
        int mid = (l + r) / 2;
        t[p].l = build(l, mid, a);
        t[p].r = build(mid + 1, r, a);
        return p;
    }

    void init(const V < ll > &a) {
        n = int(a.size()) - 1;
        t.assign(1, Node());
        root = n ? build(1, n, a) : 0;
    }

    int modify(int p, int l, int r, int pos, ll value) {
        int q = int(t.size());
        Node u = t[p];
        t.push_back(u);
        if (l == r) {
            t[q].val = value;
            return q;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) t[q].l = modify(t[p].l, l, mid, pos, value);
        else t[q].r = modify(t[p].r, mid + 1, r, pos, value);
        return q;
    }

    ll query(int p, int l, int r, int pos) {
        if (l == r) return t[p].val;
        int mid = (l + r) / 2;
        if (pos <= mid) return query(t[p].l, l, mid, pos);
        return query(t[p].r, mid + 1, r, pos);
    }

    int modify(int p, int pos, ll value) { return modify(p, 1, n, pos, value); }

    ll query(int p, int pos) { return query(p, 1, n, pos); }
};

/*
PArray st; st.init({0, 3, 1, 4});
int p = st.modify(st.root, 2, 9);
ll old = st.query(st.root, 2), now = st.query(p, 2);
*/
