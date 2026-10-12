#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 动态森林：顶点 1..n，点权、连边 / 删边、路径和；不维护子树信息。
// init(a)，a[0] 占位；link / cut 返回是否成功；query 要求两点连通。
// 单次操作均摊 O(log n)，空间 O(n)，所有 sum 不溢出 ll。
// Splay 的祖先标记用递归下传；深链验证时提高栈限制。
struct LCT {
    struct Node { int ch[2]{}, fa = 0; bool rev = false; ll val = 0, sum = 0; };
    V < Node > tr;

    void init(const V < ll > &a) {
        tr.assign(a.size(), {});
        for (int i = 1; i < int(a.size()); ++ i) tr[i].val = tr[i].sum = a[i];
    }
    bool root(int x) {
        int p = tr[x].fa;
        return ! p || (tr[p].ch[0] != x && tr[p].ch[1] != x);
    }
    void pull(int x) {
        tr[x].sum = tr[tr[x].ch[0]].sum + tr[x].val + tr[tr[x].ch[1]].sum;
    }
    void reverse(int x) {
        if (x) swap(tr[x].ch[0], tr[x].ch[1]), tr[x].rev ^= 1;
    }
    void push(int x) {
        if (tr[x].rev) {
            reverse(tr[x].ch[0]); reverse(tr[x].ch[1]); tr[x].rev = false;
        }
    }
    void down(int x) { if (! root(x)) down(tr[x].fa); push(x); }

    void rotate(int x) {
        int y = tr[x].fa, z = tr[y].fa, d = tr[y].ch[1] == x;
        int b = tr[x].ch[d ^ 1];
        if (! root(y)) tr[z].ch[tr[z].ch[1] == y] = x;
        tr[x].fa = z; tr[x].ch[d ^ 1] = y; tr[y].fa = x;
        tr[y].ch[d] = b;
        if (b) tr[b].fa = y;
        pull(y); pull(x);
    }
    void splay(int x) {
        down(x);
        while (! root(x)) {
            int y = tr[x].fa, z = tr[y].fa;
            if (! root(y))
                rotate((tr[y].ch[1] == x) == (tr[z].ch[1] == y) ? y : x);
            rotate(x);
        }
    }
    void access(int x) {
        for (int y = 0, u = x; u; y = u, u = tr[u].fa) {
            splay(u); tr[u].ch[1] = y;
            if (y) tr[y].fa = u;
            pull(u);
        }
        splay(x);
    }
    void make_root(int x) { access(x); reverse(x); }
    int find_root(int x) {
        access(x); push(x);
        while (tr[x].ch[0]) x = tr[x].ch[0], push(x);
        splay(x);
        return x;
    }
    bool same(int u, int v) { return u == v || find_root(u) == find_root(v); }
    bool link(int u, int v) {
        make_root(u);
        if (find_root(v) == u) return false;
        tr[u].fa = v;
        return true;
    }
    bool cut(int u, int v) {
        make_root(u); access(v);
        if (tr[v].ch[0] != u || tr[u].ch[1]) return false;
        tr[v].ch[0] = tr[u].fa = 0; pull(v);
        return true;
    }
    void modify(int u, ll val) { access(u); tr[u].val = val; pull(u); }
    ll query(int u, int v) { make_root(u); access(v); return tr[v].sum; }
};

// LCT tree; tree.init(a); tree.link(u, v); tree.modify(u, value);
// if (tree.same(u, v)) ll_sum = tree.query(u, v); tree.cut(u, v);
