#include <cassert>
#include "../../template/ds/RollbackDSU.cpp"
#include "../../template/math/PollardRho.cpp"
#include "../../template/math/PrimitiveRoot.cpp"
#include "../../template/math/BSGS.cpp"
#include "../../template/string/KMP.cpp"
#include "../../template/string/Manacher.cpp"
#include "../../template/string/SuffixArray.cpp"
#include "../../template/string/ACAutomaton.cpp"
#include "../../template/string/SAM.cpp"
#include "../../template/string/Trie.cpp"
#include "../../template/others/CartesianTree.cpp"
#include "../../template/others/SlidingWindow.cpp"
#include "../../template/others/SparseTable.cpp"
#include "../../template/others/Mo.cpp"

void Mo::add(int) { assert(false); }
void Mo::del(int) { assert(false); }
void Mo::answer(int) { assert(false); }

void check_rollback(mt19937_64 &rng) {
    for (int rep = 0; rep < 1000; ++ rep) {
        int n = 1 + rng() % 20;
        RollbackDSU dsu(n);
        V < V < int > > states(1, V < int > (n + 1));
        iota(states[0].begin(), states[0].end(), 0);
        for (int step = 0; step < 100; ++ step) {
            if (rng() % 4 == 0) {
                int snap = rng() % states.size();
                dsu.rollback(snap); states.resize(snap + 1);
            } else {
                auto a = states.back(); int u = 1 + rng() % n, v = 1 + rng() % n;
                bool expected = a[u] != a[v];
                assert(dsu.merge(u, v) == expected);
                int from = a[v], to = a[u];
                for (int i = 1; i <= n; ++ i) if (a[i] == from) a[i] = to;
                states.push_back(a);
            }
            assert(dsu.snapshot() == int(states.size()) - 1);
            const auto &a = states.back();
            for (int u = 1; u <= n; ++ u) {
                int size = 0;
                for (int v = 1; v <= n; ++ v) {
                    assert((dsu.find(u) == dsu.find(v)) == (a[u] == a[v]));
                    size += a[u] == a[v];
                }
                assert(dsu.siz[dsu.find(u)] == size);
            }
        }
        dsu.rollback(0);
        for (int u = 1; u <= n; ++ u) assert(dsu.fa[u] == u && dsu.siz[u] == 1);
    }
    RollbackDSU dsu(1);
    assert(! dsu.merge(1, 1) && dsu.snapshot() == 1);
    dsu.rollback(0); dsu.init(0); assert(dsu.snapshot() == 0);
}

void check_number(mt19937_64 &rng) {
    V < ull > primes;
    for (ull n = 2; n <= 1000; ++ n) {
        bool prime = true;
        for (ull d = 2; d * d <= n; ++ d) if (n % d == 0) prime = false;
        if (prime) primes.push_back(n);
    }
    for (int rep = 0; rep < 1500; ++ rep) {
        ull n = rng() % 1000000, x = n;
        V < ull > expected;
        for (ull d = 2; d <= x / d; ++ d)
            while (x % d == 0) expected.push_back(d), x /= d;
        if (x > 1) expected.push_back(x);
        assert(factor(n) == expected);
        assert(is_prime(n) == (expected.size() == 1 && expected[0] == n));
        ull p = primes[rng() % primes.size()], g = primitive_root(p), cur = 1;
        set < ull > residues;
        for (ull i = 0; i < p - 1; ++ i) residues.insert(cur), cur = cur * g % p;
        assert(residues.size() == p - 1 && cur == 1);
        ll mod = 1 + rng() % 300, a = rng() % mod, b = rng() % mod;
        V < ll > first(mod, -1); ll value = 1 % mod;
        for (int e = 0; first[value] == -1; ++ e) {
            first[value] = e; value = value * a % mod;
        }
        assert(exbsgs(a, b, mod) == first[b]);
        if (gcd(a, mod) == 1) assert(bsgs(a, b, mod) == first[b]);
    }
}

void check_strings() {
    V < string > cases{"", "a", "aaaaaa", "abababab", "abcdef", "abacaba",
                       string{char(0), char(255), char(128), char(0)}};
    for (const string &s : cases) {
        SA sa; sa.build(s); sa.build(s);
        V < int > want(s.size()); iota(want.begin(), want.end(), 0);
        sort(want.begin(), want.end(), [&](int a, int b) {
            return lexicographical_compare(s.begin() + a, s.end(), s.begin() + b, s.end(),
                [](char x, char y) { return (unsigned char)x < (unsigned char)y; });
        });
        assert(sa.sa == want);
        Manacher man(s); int longest = 0;
        for (int l = 0; l < int(s.size()); ++ l) for (int r = l; r < int(s.size()); ++ r) {
            string t = s.substr(l, r - l + 1), rev = t; reverse(rev.begin(), rev.end());
            assert(man.query(l, r) == (t == rev));
            if (t == rev) longest = max(longest, r - l + 1);
        }
        assert(man.longest().second == longest && man.query(1, 0));
        for (const string &t : cases) {
            V < int > matches;
            for (int i = 0; i + int(t.size()) <= int(s.size()); ++ i)
                if (s.compare(i, t.size(), t) == 0) matches.push_back(i);
            assert(kmp(s, t) == matches);
        }
    }
    AC ac; ac.build(); assert(ac.query("").empty());
    ac.insert("a"); ac.insert("aa"); ac.insert("a"); ac.build(); ac.build();
    assert(ac.query("aaaa") == (V < ll > {4, 3, 4}));
    assert(ac.query("") == (V < ll > {0, 0, 0}));
    SAM sam; sam.build("aaaaaa"); assert(sam.query() == 12);
    sam.build("abcdef"); assert(sam.query() == 0);
    sam.build(""); assert(sam.query() == 0);
    Trie trie; trie.insert(""); trie.insert("a"); trie.insert("a");
    assert(trie.prefix("") == 3 && trie.count("a") == 2 && trie.count("b") == 0);
    trie.clear(); assert(trie.prefix("") == 0);
}

void check_arrays() {
    for (V < ll > a : V < V < ll > > {{0}, {0, 7}, {0, 3, 3, 3},
            {0, 1, 2, 3, 4}, {0, 4, 3, 2, 1}, {0, LLONG_MIN, 0, LLONG_MAX}}) {
        Cartesian tree; tree.build(a); int n = int(a.size()) - 1;
        V < int > ord;
        auto dfs = [&](auto &&self, int u) -> void {
            if (! u) return;
            self(self, tree.ls[u]); ord.push_back(u); self(self, tree.rs[u]);
            for (int v : {tree.ls[u], tree.rs[u]}) if (v)
                assert(tree.fa[v] == u && a[u] <= a[v]);
        };
        dfs(dfs, tree.root); assert(int(ord.size()) == n);
        for (int i = 0; i < n; ++ i) assert(ord[i] == i + 1);
        ST st; st.build(a); st.build(a);
        V < ll > data(a.begin() + 1, a.end());
        for (int k = 1; k <= n; ++ k) {
            auto low = window_min(data, k), high = window_max(data, k);
            for (int i = 0; i + k <= n; ++ i) {
                assert(low[i] == *min_element(data.begin() + i, data.begin() + i + k));
                assert(high[i] == *max_element(data.begin() + i, data.begin() + i + k));
                assert(st.query(i + 1, i + k) == high[i]);
            }
        }
    }
    Mo mo;
    mo.solve(0, {});
}

int main() {
    mt19937_64 rng(2026101203);
    check_rollback(rng); check_number(rng); check_strings(); check_arrays();
    cout << "PASS: 1000 rollback histories, 1500 independent number cases and deterministic string/array edges\n";
}
