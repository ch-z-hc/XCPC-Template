/*
字母为 [0, sigma) 内的整数。先插入所有非空模式串，再 build；继续插入后需重新 build。
持久化线段树共享失配转移，模式总长 n、文本长 m，建表 O(n log(sigma + 1))，
匹配计数 O(m log(sigma + 1) + n)，返回值按 insert 给出的终点编号索引。
*/

#include <bits/stdc++.h>

#define V vector
using namespace std;

struct SparseAC {
    struct Node {
        map < int, int > ch;
        int fail = 0, root = 0;
    };
    struct Seg { int l = 0, r = 0, v = 0; };
    int sigma;
    V < Node > tr = V < Node >(1);
    V < Seg > seg;
    V < int > ord;
    explicit SparseAC(int sigma) : sigma(sigma) {}

    int insert(const V < int > &a) {
        int u = 0;
        for (int c : a) {
            int v = tr[u].ch[c];
            if (!v) {
                tr[u].ch[c] = v = int(tr.size());
                tr.emplace_back();
            }
            u = v;
        }
        return u;
    }

    int set_edge(int p, int l, int r, int c, int v) {
        int q = int(seg.size());
        seg.push_back(seg[p]);
        if (l == r) seg[q].v = v;
        else {
            int mid = l + (r - l) / 2;
            if (c <= mid) seg[q].l = set_edge(seg[p].l, l, mid, c, v);
            else seg[q].r = set_edge(seg[p].r, mid + 1, r, c, v);
        }
        return q;
    }

    int step(int u, int c) {
        int p = tr[u].root, l = 0, r = sigma - 1;
        while (p && l < r) {
            int mid = l + (r - l) / 2;
            if (c <= mid) p = seg[p].l, r = mid;
            else p = seg[p].r, l = mid + 1;
        }
        return seg[p].v;
    }

    void build() {
        ord = {0};
        seg.assign(1, {});
        for (size_t i = 0; i < ord.size(); i ++) {
            int u = ord[i], f = tr[u].fail;
            tr[u].root = u ? tr[f].root : 0;
            for (auto [c, v] : tr[u].ch) {
                tr[v].fail = u ? step(f, c) : 0;
                tr[u].root = set_edge(tr[u].root, 0, sigma - 1, c, v);
                ord.push_back(v);
            }
        }
    }

    V < int > count(const V < int > &s) {
        V < int > cnt(tr.size());
        int u = 0;
        for (int c : s) cnt[u = step(u, c)] ++;
        for (int i = int(ord.size()) - 1; i > 0; i --)
            cnt[tr[ord[i]].fail] += cnt[ord[i]];
        return cnt;
    }
};

/*
SparseAC ac(1000000000); int id = ac.insert({17, 910});
ac.build(); int v = ac.step(0, 17);
int hits = ac.count({17, 910, 17, 910})[id];
*/
