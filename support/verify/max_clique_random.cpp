#include <cassert>
#include "../../template/graph/MaxClique.cpp"

int brute(const MaxClique &mc, bool independent) {
    int n = mc.n, ans = 0;
    V < unsigned > mask(n);
    for (int u = 0; u < n; ++ u) for (int v = 0; v < n; ++ v)
        if (u != v && (bool(mc.g[u + 1][v + 1]) != independent)) mask[u] |= 1u << v;
    V < char > ok(1u << n); ok[0] = 1;
    for (unsigned s = 1; s < (1u << n); ++ s) {
        int u = __builtin_ctz(s); unsigned rest = s & (s - 1);
        ok[s] = ok[rest] && (rest & mask[u]) == rest;
        if (ok[s]) ans = max(ans, __builtin_popcount(s));
    }
    return ans;
}

void check(MaxClique &mc) {
    for (bool independent : {false, true}) {
        auto ans = independent ? mc.max_independent_set() : mc.max_clique();
        assert(int(ans.size()) == brute(mc, independent));
        set < int > seen;
        for (int u : ans) {
            assert(1 <= u && u <= mc.n && seen.insert(u).second);
            for (int v : seen) if (u != v) assert(bool(mc.g[u][v]) != independent);
        }
    }
    assert(int(mc.max_clique().size()) == brute(mc, 0));
}

int main() {
    MaxClique empty; check(empty);
    for (int mask = 0; mask < (1 << 10); ++ mask) {
        MaxClique mc(5); int bit = 0;
        for (int u = 1; u <= 5; ++ u) for (int v = u + 1; v <= 5; ++ v)
            if (mask >> bit ++ & 1) mc.add_edge(u, v);
        check(mc);
    }
    mt19937 rng(20261013);
    for (int test = 0; test < 1200; ++ test) {
        int n = int(rng() % 16) + 1, density = rng() % 101;
        MaxClique mc(n);
        for (int u = 1; u <= n; ++ u) for (int v = 1; v <= n; ++ v)
            if (int(rng() % 100) < density) mc.add_edge(u, v);
        check(mc);
        mc.init(3); check(mc);
    }
    MaxClique mc(100);
    assert(mc.max_clique().size() == 1 && mc.max_independent_set().size() == 100);
    for (int u = 1; u <= 100; ++ u) for (int v = 1; v <= 100; ++ v) mc.add_edge(u, v);
    assert(mc.max_clique().size() == 100 && mc.max_independent_set().size() == 1);
    cout << "max clique random tests passed\n";
}
