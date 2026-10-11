#include <cassert>
#include "../../template/graph/LowerBoundFlow.cpp"
#include "../../template/graph/KM.cpp"
#include "../../template/graph/HopcroftKarp.cpp"

void check_flow(const LRFlow &flow, bool ok, ll ans, int s = 0, int t = 0) {
    if (! ok) { assert(flow.f.empty()); return; }
    V < ll > bal(flow.n + 1);
    assert(flow.f.size() == flow.edges.size());
    for (int i = 0; i < int(flow.edges.size()); ++ i) {
        auto e = flow.edges[i];
        ll val = flow.f[i];
        assert(e.lo <= val && val <= e.hi);
        bal[e.u] += val; bal[e.v] -= val;
    }
    for (int u = 1; u <= flow.n; ++ u)
        if (u != s && u != t) assert(bal[u] == 0);
    if (s) assert(bal[s] == ans && bal[t] == -ans);
}

void test_lr(mt19937 &rng) {
    for (int test = 0; test < 1600; ++ test) {
        int n = int(rng() % 4) + 2, m = rng() % 8;
        LRFlow flow(n);
        for (int i = 0; i < m; ++ i) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            ll hi = rng() % 4, lo = rng() % (hi + 1);
            assert(flow.add_edge(u, v, lo, hi) == i);
        }
        bool cyc = false, ok = false;
        ll mn = LLONG_MAX, mx = LLONG_MIN;
        V < ll > bal(n + 1);
        auto dfs = [&](auto &&self, int k) -> void {
            if (k == m) {
                bool all = true, mid = true;
                for (int u = 1; u <= n; ++ u) {
                    all &= bal[u] == 0;
                    if (u != 1 && u != n) mid &= bal[u] == 0;
                }
                cyc |= all;
                if (mid) ok = true, mn = min(mn, bal[1]), mx = max(mx, bal[1]);
                return;
            }
            auto e = flow.edges[k];
            for (ll val = e.lo; val <= e.hi; ++ val) {
                bal[e.u] += val; bal[e.v] -= val;
                self(self, k + 1);
                bal[e.u] -= val; bal[e.v] += val;
            }
        };
        dfs(dfs, 0);
        for (int rep = 0; rep < 2; ++ rep) {
            auto ans = flow.solve();
            assert(ans.first == cyc && ans.second == 0);
            check_flow(flow, ans.first, 0);
            for (int mode : {0, 1, -1}) {
                ans = flow.solve(1, n, mode);
                assert(ans.first == ok);
                if (ok && mode == 1) assert(ans.second == mx);
                if (ok && mode == -1) assert(ans.second == mn);
                check_flow(flow, ans.first, ans.second, 1, n);
            }
        }
    }
    LRFlow flow(2);
    flow.add_edge(2, 1, 3, 5);
    assert(flow.solve(1, 2, 1) == make_pair(true, -3LL));
    check_flow(flow, true, -3, 1, 2);
    assert(flow.solve(1, 2, -1) == make_pair(true, -5LL));
    check_flow(flow, true, -5, 1, 2);
    assert(! flow.solve().first);
    flow.init(2);
    ll cap = LLONG_MAX / 8;
    flow.add_edge(1, 2, cap, cap); flow.add_edge(2, 1, 0, cap);
    assert(flow.solve(1, 2, 1) == make_pair(true, cap));
    check_flow(flow, true, cap, 1, 2);
    assert(flow.solve(1, 2, -1) == make_pair(true, 0LL));
    check_flow(flow, true, 0, 1, 2);
    flow.init(3);
    flow.add_edge(1, 2, 2, 2);
    assert(! flow.solve(1, 3).first);
    assert(flow.f.empty());
    flow.add_edge(2, 3, 2, 2);
    assert(flow.solve(1, 3) == make_pair(true, 2LL));
    check_flow(flow, true, 2, 1, 3);
}

void check_match(const V < int > &ml, const V < int > &mr, int ans) {
    int cnt = 0;
    for (int u = 1; u < int(ml.size()); ++ u) if (ml[u]) {
        assert(ml[u] > 0 && ml[u] < int(mr.size()));
        assert(mr[ml[u]] == u);
        cnt ++;
    }
    for (int v = 1; v < int(mr.size()); ++ v) if (mr[v]) {
        assert(mr[v] > 0 && mr[v] < int(ml.size()));
        assert(ml[mr[v]] == v);
    }
    assert(cnt == ans);
}

void test_km(mt19937 &rng) {
    for (int test = 0; test < 6000; ++ test) {
        int n = rng() % 5, m = rng() % 6;
        KM km(n, m);
        for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= m; ++ v) if (rng() % 3) {
                km.add_edge(u, v, int(rng() % 31) - 15);
                if (rng() % 4 == 0) km.add_edge(u, v, int(rng() % 31) - 15);
            }
        ll best = LLONG_MIN;
        V < bool > vis(m + 1);
        auto dfs = [&](auto &&self, int u, ll ans) -> void {
            if (u > n) { best = max(best, ans); return; }
            for (int v = 1; v <= m; ++ v) if (! vis[v] && km.op[u][v]) {
                vis[v] = true;
                self(self, u + 1, ans + km.w[u][v]);
                vis[v] = false;
            }
        };
        dfs(dfs, 1, 0);
        for (int rep = 0; rep < 2; ++ rep) {
            auto ans = km.solve();
            assert(ans.first == (best != LLONG_MIN));
            if (ans.first) {
                assert(ans.second == best);
                check_match(km.ml, km.mr, n);
                ll sum = 0;
                for (int u = 1; u <= n; ++ u) {
                    assert(km.op[u][km.ml[u]]);
                    sum += km.w[u][km.ml[u]];
                }
                assert(sum == best);
            } else {
                for (int v : km.ml) assert(! v);
                for (int u : km.mr) assert(! u);
            }
        }
    }
    KM km(1, 1);
    km.add_edge(1, 1, LLONG_MIN);
    assert(km.solve() == make_pair(true, LLONG_MIN));
    km.init(2, 2);
    km.add_edge(1, 1, LLONG_MAX); km.add_edge(2, 2, LLONG_MIN);
    assert(km.solve() == make_pair(true, -1LL));
    km.init(2, 3);
    km.add_edge(1, 1, -5); km.add_edge(2, 1, -7);
    assert(! km.solve().first);
    km.add_edge(2, 3, -9);
    assert(km.solve() == make_pair(true, -14LL));
    check_match(km.ml, km.mr, 2);
}

void test_hk(mt19937 &rng) {
    for (int test = 0; test < 6000; ++ test) {
        int n = rng() % 8, m = rng() % 8;
        HK hk(n, m);
        for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= m; ++ v) if (rng() % 3 == 0) {
                hk.add_edge(u, v);
                if (rng() % 3 == 0) hk.add_edge(u, v);
            }
        V < bool > dp(1 << m);
        dp[0] = true;
        for (int u = 1; u <= n; ++ u) {
            auto ndp = dp;
            for (int mask = 0; mask < (1 << m); ++ mask) if (dp[mask])
                for (int v : hk.g[u]) if (! (mask >> (v - 1) & 1))
                    ndp[mask | (1 << (v - 1))] = true;
            dp.swap(ndp);
        }
        int best = 0;
        for (int mask = 0; mask < (1 << m); ++ mask) if (dp[mask])
            best = max(best, __builtin_popcount(unsigned(mask)));
        for (int rep = 0; rep < 2; ++ rep) {
            assert(hk.solve() == best);
            check_match(hk.ml, hk.mr, best);
            for (int u = 1; u <= n; ++ u) if (hk.ml[u])
                assert(find(hk.g[u].begin(), hk.g[u].end(), hk.ml[u])
                       != hk.g[u].end());
        }
        hk.init(2, 3); hk.add_edge(1, 3); hk.add_edge(2, 3);
        assert(hk.solve() == 1);
        hk.add_edge(2, 2);
        assert(hk.solve() == 2);
        check_match(hk.ml, hk.mr, 2);
    }
    int n = 200000;
    HK hk(n, n);
    for (int u = 1; u < n; ++ u) {
        hk.add_edge(u, u + 1); hk.add_edge(u, u);
    }
    hk.add_edge(n, n);
    assert(hk.solve() == n);
    check_match(hk.ml, hk.mr, n);
}

int main() {
    mt19937 rng(19413);
    test_lr(rng);
    test_km(rng);
    test_hk(rng);
    cout << "PASS: LRFlow / KM / HK random brute force and regressions\n";
}
