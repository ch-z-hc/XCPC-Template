#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
小根堆，根 0 表示空。push(x) 建一个单点堆，同值时编号小的点先出堆。
merge 只合并不相交的堆。merge 和 pop 都要接住新根，top 和 pop 只能用于非空堆。
合并、弹出 O(log n)，取顶 O(1)，插入均摊 O(1)。按任意节点找堆需另维护并查集。
*/
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

/*
LeftistHeap heap; int root = 0;
root = heap.merge(root, heap.push(x)); root = heap.pop(root);
*/
