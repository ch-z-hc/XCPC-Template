#include <bits/stdc++.h>
#include "../../template/ds/可持久化二进制字典树.cpp"

int main() {
    mt19937_64 rng(20261014);
    for (int test = 0; test < 300; ++ test) {
        PXorTrie trie;
        V < int > root(1), par(1);
        V < unsigned long long > val(1);
        for (int i = 1; i <= 180; ++ i) {
            int p = rng() % root.size();
            unsigned long long x = rng();
            if (i % 5 == 0) x = 0;
            if (i % 7 == 0) x = ~0ull;
            if (i % 11 == 0) x = 1ull << 63;
            root.push_back(trie.insert(root[p], x));
            par.push_back(p); val.push_back(x);
            for (int repeat = 0; repeat < 6; ++ repeat) {
                int r = rng() % root.size(), l = r;
                int steps = rng() % (i + 1);
                while (steps -- && l) l = par[l];
                unsigned long long q = rng(), want = 0;
                bool found = 0;
                for (int v = r; v != l; v = par[v]) {
                    want = max(want, val[v] ^ q); found = 1;
                }
                unsigned long long ans;
                assert(trie.max_xor(root[l], root[r], q, ans) == found);
                if (found) assert(ans == want);
            }
        }
        assert(trie.tr.size() == 1 + 65 * 180);
        trie.init(); assert(trie.tr.size() == 1);
        unsigned long long ans;
        assert(! trie.max_xor(0, 0, 0, ans));
        int one = trie.insert(0, ~0ull);
        assert(trie.max_xor(0, one, 0, ans) && ans == ~0ull);
        assert(trie.max_xor(0, one, ~0ull, ans) && ans == 0ull);
        assert(! trie.max_xor(one, one, 0, ans));
    }
    PXorTrie trie;
    V < int > root(1);
    V < unsigned long long > a(100);
    for (int i = 1; i < int(a.size()); ++ i) {
        a[i] = rng(); root.push_back(trie.insert(root.back(), a[i]));
    }
    for (int l = 1; l < int(a.size()); ++ l) for (int r = l; r < int(a.size()); ++ r) {
        unsigned long long x = rng(), want = 0;
        for (int i = l; i <= r; ++ i) want = max(want, a[i] ^ x);
        unsigned long long ans;
        assert(trie.max_xor(root[l - 1], root[r], x, ans) && ans == want);
    }
    cout << "persistent xor trie random tests passed\n";
}
