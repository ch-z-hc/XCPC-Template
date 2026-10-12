#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 可持久化数组：从任意历史根单点赋值，查询任意历史根的单点值。
// a[1..n]，a[0] 为占位；不使用模数，值可为任意 ll。
// 建树 O(n)，单次修改、查询 O(log n)，每次修改新增 O(log n) 节点。
// init 重置节点池，root 为初始根；调用者自行保存各版本的根。
// init({0}) 可建立空数组；修改和查询要求非空且根、下标有效。
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
        if (l == r) { t[p].val = a[l]; return p; }
        int mid = (l + r) >> 1;
        int x = build(l, mid, a), y = build(mid + 1, r, a);
        t[p].l = x; t[p].r = y;
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
        if (l == r) { t[q].val = value; return q; }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            int x = modify(t[p].l, l, mid, pos, value);
            t[q].l = x;
        } else {
            int x = modify(t[p].r, mid + 1, r, pos, value);
            t[q].r = x;
        }
        return q;
    }

    ll query(int p, int l, int r, int pos) {
        if (l == r) return t[p].val;
        int mid = (l + r) >> 1;
        if (pos <= mid) return query(t[p].l, l, mid, pos);
        return query(t[p].r, mid + 1, r, pos);
    }

    int modify(int p, int pos, ll value) {
        return modify(p, 1, n, pos, value);
    }

    ll query(int p, int pos) { return query(p, 1, n, pos); }
};

// 调用示例：
// PArray st; st.init({0, 3, 1, 4});
// int p = st.modify(st.root, 2, 9);
// int q = st.modify(st.root, 1, 7); // 从初始版本另开分支
// ll old = st.query(st.root, 2), now = st.query(p, 2); // 1, 9
// ll other = st.query(q, 1); // 7，st.root 和已有版本均未改变
