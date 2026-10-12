#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 小根左偏树；节点编号 1-based，0 为空，同值时较小编号优先。
// merge、pop 返回新根且消耗旧根；只合并两个不相交的堆。
// push 返回一个新单点堆的根；访问 top / pop 时根必须非零。
// merge / pop 为 O(log n)，push / top 为 O(1) 均摊，空间 O(插入总数)。
// 按任意节点查所在堆时，需另维护并查集与已删除标记。
struct LeftistHeap {
    struct Node {
        ll val = 0;
        int l = 0, r = 0, dis = 0;
    };
    V < Node > tr;

    LeftistHeap() { init(); }
    void init() { tr.assign(1, Node{}); tr[0].dis = -1; }

    int push(ll val) {
        tr.push_back({val, 0, 0, 0});
        return int(tr.size()) - 1;
    }

    ll top(int root) { return tr[root].val; }

    int merge(int u, int v) {
        if (! u || ! v) return u | v;
        if (tr[u].val > tr[v].val ||
            (tr[u].val == tr[v].val && u > v)) swap(u, v);
        tr[u].r = merge(tr[u].r, v);
        if (tr[tr[u].l].dis < tr[tr[u].r].dis) swap(tr[u].l, tr[u].r);
        tr[u].dis = tr[tr[u].r].dis + 1;
        return u;
    }

    int pop(int root) {
        int ans = merge(tr[root].l, tr[root].r);
        tr[root].l = tr[root].r = tr[root].dis = 0;
        return ans;
    }
};

// 调用：LeftistHeap heap; int root = 0;
// root = heap.merge(root, heap.push(x)); root = heap.pop(root);
