#include <bits/stdc++.h>

#include "../../template/ds/FHQTreap.cpp"
#include "../../template/ds/WeightedDSU.cpp"
#include "../../template/ds/PersistentSegTree.cpp"
#include "../../template/ds/XorTrie.cpp"

using namespace std;

mt19937_64 rng(18273645);

ll key() {
    switch (rng() % 12) {
        case 0: return LLONG_MIN;
        case 1: return LLONG_MAX;
        default: return ll(rng() % 21) - 10;
    }
}

void test_treap() {
    Treap t;
    for (int z = 0; z < 60; ++ z) {
        t.init();
        multiset < ll > s;
        assert(! t.kth(1) && ! t.pre(0) && ! t.suf(0));
        assert(! t.erase(0));
        for (int q = 0; q < 1000; ++ q) {
            ll x = key();
            if (rng() % 3) t.insert(x), s.insert(x);
            else {
                auto it = s.find(x);
                assert(t.erase(x) == (it != s.end()));
                if (it != s.end()) s.erase(it);
            }
            assert(t.size() == int(s.size()));
            assert(t.rank(x) == distance(s.begin(), s.lower_bound(x)) + 1);
            optional < ll > a, b, c;
            auto it = s.lower_bound(x);
            if (it != s.begin()) a = *prev(it);
            it = s.upper_bound(x);
            if (it != s.end()) b = *it;
            int k = int(rng() % (s.size() + 3));
            if (k >= 1 && k <= int(s.size())) c = *next(s.begin(), k - 1);
            assert(t.pre(x) == a && t.suf(x) == b && t.kth(k) == c);
        }
    }
}

void test_seq() {
    SeqTreap t;
    for (int z = 0; z < 60; ++ z) {
        t.init();
        V < ll > a;
        for (int q = 0; q < 1000; ++ q) {
            int op = rng() % 4;
            if (a.empty() || op == 0) {
                int pos = rng() % (a.size() + 1);
                ll x = ll(rng() % 2001) - 1000;
                t.insert(pos + 1, x); a.insert(a.begin() + pos, x);
            } else if (op == 1) {
                int pos = rng() % a.size();
                t.erase(pos + 1); a.erase(a.begin() + pos);
            } else {
                int l = rng() % a.size(), r = rng() % a.size();
                if (l > r) swap(l, r);
                if (op == 2) {
                    t.reverse(l + 1, r + 1);
                    reverse(a.begin() + l, a.begin() + r + 1);
                } else {
                    ll ans = accumulate(a.begin() + l, a.begin() + r + 1, 0ll);
                    assert(t.query(l + 1, r + 1) == ans);
                }
            }
            assert(t.size() == int(a.size()));
            if (! a.empty()) {
                int pos = rng() % a.size();
                assert(t.at(pos + 1) == a[pos]);
                assert(t.query(1, int(a.size())) == accumulate(a.begin(), a.end(), 0ll));
            }
        }
    }
    t.init(); t.insert(1, LLONG_MIN); assert(t.at(1) == LLONG_MIN);
    t.erase(1); t.insert(1, LLONG_MAX); assert(t.at(1) == LLONG_MAX);
}

optional < ll > relation(const V < V < pair < int, ll > > > &g, int s, int t) {
    V < int > vis(g.size());
    V < ll > dis(g.size());
    queue < int > q;
    q.push(s); vis[s] = 1;
    while (! q.empty()) {
        int u = q.front(); q.pop();
        if (u == t) return dis[u];
        for (auto [v, w] : g[u]) if (! vis[v]) {
            vis[v] = 1; dis[v] = dis[u] + w; q.push(v);
        }
    }
    return nullopt;
}

optional < unsigned long long > xor_relation(
    const V < V < pair < int, unsigned long long > > > &g, int s, int t) {
    V < int > vis(g.size());
    V < unsigned long long > dis(g.size());
    queue < int > q;
    q.push(s); vis[s] = 1;
    while (! q.empty()) {
        int u = q.front(); q.pop();
        if (u == t) return dis[u];
        for (auto [v, w] : g[u]) if (! vis[v]) {
            vis[v] = 1; dis[v] = dis[u] ^ w; q.push(v);
        }
    }
    return nullopt;
}

void test_dsu() {
    WDSU a;
    XDSU b;
    for (int z = 0; z < 80; ++ z) {
        int n = rng() % 30 + 1;
        a.init(n); b.init(n);
        V < V < pair < int, ll > > > g(n + 1);
        V < V < pair < int, unsigned long long > > > h(n + 1);
        for (int q = 0; q < 500; ++ q) {
            int u = rng() % n + 1, v = rng() % n + 1;
            auto x = relation(g, u, v);
            auto y = xor_relation(h, u, v);
            assert(a.query(u, v) == x && b.query(u, v) == y);
            ll w = ll(rng() % 101) - 50;
            unsigned long long t = rng();
            if (x && rng() % 2) w = *x;
            if (y && rng() % 2) t = *y;
            bool ok = ! x || *x == w, op = ! y || *y == t;
            assert(a.merge(u, v, w) == ok && b.merge(u, v, t) == op);
            if (ok) g[u].push_back({v, w}), g[v].push_back({u, -w});
            if (op) h[u].push_back({v, t}), h[v].push_back({u, t});
        }
    }
    b.init(2);
    assert(b.merge(1, 2, 1ull << 63));
    assert(b.query(1, 2) == (1ull << 63));
}

void test_pst() {
    PST t;
    t.build({0});
    for (int z = 0; z < 120; ++ z) {
        int n = rng() % 100 + 1;
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = key();
        t.build(a);
        for (int q = 0; q < 200; ++ q) {
            int l = rng() % n + 1, r = rng() % n + 1;
            if (l > r) swap(l, r);
            V < ll > b(a.begin() + l, a.begin() + r + 1);
            sort(b.begin(), b.end());
            int k = rng() % b.size() + 1;
            ll x = key();
            assert(t.kth(l, r, k) == b[k - 1]);
            assert(t.count(l, r, x) == upper_bound(b.begin(), b.end(), x) - b.begin());
        }
    }
}

void test_xor_trie() {
    XorTrie t;
    for (int z = 0; z < 60; ++ z) {
        t.init();
        multiset < unsigned long long > s;
        assert(! t.max_xor(0) && ! t.erase(0));
        for (int q = 0; q < 1000; ++ q) {
            unsigned long long x = rng() % 16;
            if (rng() % 2) x |= 1ull << 63;
            if (rng() % 3) t.insert(x), s.insert(x);
            else {
                auto it = s.find(x);
                assert(t.erase(x) == (it != s.end()));
                if (it != s.end()) s.erase(it);
            }
            x = rng();
            optional < unsigned long long > ans;
            for (auto y : s) if (! ans || (x ^ y) > *ans) ans = x ^ y;
            assert(t.max_xor(x) == ans);
        }
    }
    t.init(); t.insert(0); t.insert(ULLONG_MAX);
    assert(t.max_xor(0) == ULLONG_MAX);
    assert(t.erase(ULLONG_MAX) && t.max_xor(0) == 0);
}

int main() {
    test_treap(); test_seq(); test_dsu(); test_pst(); test_xor_trie();
    cout << "PASS: Treap, SeqTreap, weighted DSU, PST and 64-bit Trie\n";
}
