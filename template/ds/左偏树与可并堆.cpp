/*
可合并小根堆，根 0 为空，同值按节点编号出堆。只合并不相交的堆，只对非空堆取顶或弹出。
建单点堆均摊 O(1)，取顶 O(1)，合并、弹出 O(log n)。
merge 和 pop 均返回新根。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct LeftistHeap {
    struct Node {
        i64 val = 0;
        int l = 0, r = 0, dis = 0;
    };
    std::vector<Node> tr;

    LeftistHeap() { init(); }
    void init() {
        tr.assign(1, Node{});
        tr[0].dis = -1;
    }

    int push(i64 val) {
        tr.push_back({val, 0, 0, 0});
        return int(tr.size()) - 1;
    }

    i64 top(int root) { return tr[root].val; }

    int merge(int u, int v) {
        if (!u || !v) return u | v;
        if (tr[u].val > tr[v].val || (tr[u].val == tr[v].val && u > v)) std::swap(u, v);
        tr[u].r = merge(tr[u].r, v);
        if (tr[tr[u].l].dis < tr[tr[u].r].dis) std::swap(tr[u].l, tr[u].r);
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
LeftistHeap heap; int a = heap.push(7), b = heap.push(3);
a = heap.merge(a, b); i64 x = heap.top(a);
a = heap.pop(a);
heap.init();
*/
