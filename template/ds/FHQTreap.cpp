#include <bits/stdc++.h>
#include <cassert>

#define V vector

typedef long long ll;

using namespace std;

// FHQ Treap：键值多重集 Treap，以及支持区间翻转、区间和的序列 SeqTreap。
// 随机优先级，操作期望 O(log n)，最坏 O(n)；空间 O(插入次数)，删除不回收。
// Treap 的 rank(x) = 小于 x 的数量 + 1，kth 从 1 开始；pre / suf 严格小于 / 大于。
// kth / pre / suf 不存在时返回 nullopt；erase 只删一份，不存在时返回 false。
// SeqTreap 下标 1-based，insert(pos, x) 在原 pos 前插入，末尾可用 size() + 1。
// 序列操作位置、闭区间必须合法，所有区间和及中间加法须不溢出 ll。
// init() 清空，可多次使用；键值支持 LLONG_MIN / LLONG_MAX。

struct Treap {
    struct Node {
        int ls = 0, rs = 0, siz = 0;
        unsigned pri = 0;
        ll val = 0;
    };
    V < Node > tr;
    int root = 0;
    mt19937 rng{unsigned(chrono::steady_clock::now().time_since_epoch().count())};

    Treap() { init(); }
    void init() { tr.assign(1, {}); root = 0; }
    int size() const { return tr[root].siz; }
    void pull(int u) { tr[u].siz = tr[tr[u].ls].siz + tr[tr[u].rs].siz + 1; }

    void split(int u, ll x, int &a, int &b) {
        if (! u) { a = b = 0; return; }
        if (tr[u].val <= x) {
            a = u; split(tr[u].rs, x, tr[u].rs, b);
        } else {
            b = u; split(tr[u].ls, x, a, tr[u].ls);
        }
        pull(u);
    }

    int merge(int a, int b) {
        if (! a || ! b) return a | b;
        if (tr[a].pri > tr[b].pri) {
            tr[a].rs = merge(tr[a].rs, b); pull(a);
            return a;
        }
        tr[b].ls = merge(a, tr[b].ls); pull(b);
        return b;
    }

    void insert(ll x) {
        int u = int(tr.size()), a, b;
        tr.push_back({0, 0, 1, unsigned(rng()), x});
        split(root, x, a, b);
        root = merge(merge(a, u), b);
    }

    int erase(int u, ll x, bool &ok) {
        if (! u) return 0;
        if (tr[u].val == x) {
            ok = 1;
            return merge(tr[u].ls, tr[u].rs);
        }
        if (x < tr[u].val) tr[u].ls = erase(tr[u].ls, x, ok);
        else tr[u].rs = erase(tr[u].rs, x, ok);
        pull(u);
        return u;
    }

    bool erase(ll x) {
        bool ok = 0;
        root = erase(root, x, ok);
        return ok;
    }

    int rank(ll x) const {
        int ans = 1, u = root;
        while (u) {
            if (tr[u].val < x) ans += tr[tr[u].ls].siz + 1, u = tr[u].rs;
            else u = tr[u].ls;
        }
        return ans;
    }

    optional < ll > kth(int k) const {
        if (k < 1 || k > size()) return nullopt;
        int u = root;
        while (u) {
            int s = tr[tr[u].ls].siz;
            if (k == s + 1) return tr[u].val;
            if (k <= s) u = tr[u].ls;
            else k -= s + 1, u = tr[u].rs;
        }
        return nullopt;
    }

    optional < ll > pre(ll x) const {
        optional < ll > ans;
        for (int u = root; u; ) {
            if (tr[u].val < x) ans = tr[u].val, u = tr[u].rs;
            else u = tr[u].ls;
        }
        return ans;
    }

    optional < ll > suf(ll x) const {
        optional < ll > ans;
        for (int u = root; u; ) {
            if (tr[u].val > x) ans = tr[u].val, u = tr[u].ls;
            else u = tr[u].rs;
        }
        return ans;
    }
};

struct SeqTreap {
    struct Node {
        int ls = 0, rs = 0, siz = 0;
        unsigned pri = 0;
        ll val = 0, sum = 0;
        bool rev = false;
    };
    V < Node > tr;
    int root = 0;
    mt19937 rng{unsigned(chrono::steady_clock::now().time_since_epoch().count())};

    SeqTreap() { init(); }
    void init() { tr.assign(1, {}); root = 0; }
    int size() const { return tr[root].siz; }
    void pull(int u) {
        tr[u].siz = tr[tr[u].ls].siz + tr[tr[u].rs].siz + 1;
        tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum + tr[u].val;
    }
    void apply(int u) {
        if (! u) return;
        swap(tr[u].ls, tr[u].rs); tr[u].rev ^= 1;
    }
    void push(int u) {
        if (! tr[u].rev) return;
        apply(tr[u].ls); apply(tr[u].rs); tr[u].rev = 0;
    }

    void split(int u, int k, int &a, int &b) {
        if (! u) { a = b = 0; return; }
        push(u);
        if (tr[tr[u].ls].siz < k) {
            a = u;
            split(tr[u].rs, k - tr[tr[u].ls].siz - 1, tr[u].rs, b);
        } else {
            b = u; split(tr[u].ls, k, a, tr[u].ls);
        }
        pull(u);
    }

    int merge(int a, int b) {
        if (! a || ! b) return a | b;
        if (tr[a].pri > tr[b].pri) {
            push(a); tr[a].rs = merge(tr[a].rs, b); pull(a);
            return a;
        }
        push(b); tr[b].ls = merge(a, tr[b].ls); pull(b);
        return b;
    }

    void insert(int pos, ll x) {
        assert(pos >= 1 && pos <= size() + 1);
        int u = int(tr.size()), a, b;
        tr.push_back({0, 0, 1, unsigned(rng()), x, x, false});
        split(root, pos - 1, a, b);
        root = merge(merge(a, u), b);
    }

    void erase(int pos) {
        assert(pos >= 1 && pos <= size());
        int a, b, c;
        split(root, pos - 1, a, b); split(b, 1, b, c);
        root = merge(a, c);
    }

    void reverse(int l, int r) {
        assert(l >= 1 && l <= r && r <= size());
        int a, b, c;
        split(root, l - 1, a, b); split(b, r - l + 1, b, c);
        apply(b); root = merge(merge(a, b), c);
    }

    ll query(int l, int r) {
        assert(l >= 1 && l <= r && r <= size());
        int a, b, c;
        split(root, l - 1, a, b); split(b, r - l + 1, b, c);
        ll ans = tr[b].sum;
        root = merge(merge(a, b), c);
        return ans;
    }

    ll at(int pos) { return query(pos, pos); }
};

// 调用示例：
// Treap t; t.insert(3); t.insert(3); t.erase(3); auto x = t.kth(1);
// SeqTreap s; s.insert(1, 2); s.insert(2, 5); s.reverse(1, 2);
// ll sum = s.query(1, 2), val = s.at(1);
