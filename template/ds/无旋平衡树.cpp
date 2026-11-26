/*
Treap 维护可重集，SeqTreap 维护可翻转序列。排名、位置从 1 数，序列区间闭合，和须放入 ll。
操作期望 O(log n)，最坏 O(n)。删除不回收节点，空间随累计插入次数增长。
Treap 的 erase 每次只删一份，rank(x) 等于严格小于 x 的元素个数加 1，pre、suf 查严格前驱和后继。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

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
    int size() { return tr[root].siz; }
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

    int rank(ll x) {
        int ans = 1, u = root;
        while (u) {
            if (tr[u].val < x) ans += tr[tr[u].ls].siz + 1, u = tr[u].rs;
            else u = tr[u].ls;
        }
        return ans;
    }

    bool kth(int k, ll &ans) {
        if (k < 1 || k > size()) return false;
        int u = root;
        while (u) {
            int s = tr[tr[u].ls].siz;
            if (k == s + 1) { ans = tr[u].val; return true; }
            if (k <= s) u = tr[u].ls;
            else k -= s + 1, u = tr[u].rs;
        }
        return false;
    }

    bool pre(ll x, ll &ans) {
        bool ok = false;
        for (int u = root; u; ) {
            if (tr[u].val < x) ans = tr[u].val, ok = true, u = tr[u].rs;
            else u = tr[u].ls;
        }
        return ok;
    }

    bool suf(ll x, ll &ans) {
        bool ok = false;
        for (int u = root; u; ) {
            if (tr[u].val > x) ans = tr[u].val, ok = true, u = tr[u].ls;
            else u = tr[u].rs;
        }
        return ok;
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
    int size() { return tr[root].siz; }
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
        apply(tr[u].ls);
        apply(tr[u].rs);
        tr[u].rev = 0;
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
            push(a);
            tr[a].rs = merge(tr[a].rs, b);
            pull(a);
            return a;
        }
        push(b);
        tr[b].ls = merge(a, tr[b].ls);
        pull(b);
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

/*
Treap t; t.insert(3); t.insert(5); bool erased = t.erase(3);
int n = t.size(), r = t.rank(5);
ll x; if (t.kth(1, x)) cout << x;
if (t.pre(6, x)) cout << x;
if (t.suf(4, x)) cout << x;
t.init();
SeqTreap s; s.insert(1, 2); s.insert(s.size() + 1, 5);
s.reverse(1, 2); ll sum = s.query(1, 2), val = s.at(1);
s.erase(1); s.init();
*/
