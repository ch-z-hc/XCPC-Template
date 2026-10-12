#include <cassert>
#include "../../template/graph/势能最短路费用流.cpp"
#include "../../template/graph/上下界最小费用流.cpp"

optional < ll > brute(const LRCostFlow &flow) {
    optional < ll > ans;
    V < ll > bal(flow.n + 1);
    auto dfs = [&](auto &&self, int id, ll cost) -> void {
        if (id == int(flow.edges.size())) {
            if (bal == flow.supply && (! ans || cost < *ans)) ans = cost;
            return;
        }
        auto &e = flow.edges[id];
        for (ll x = e.lo; x <= e.hi; ++ x) {
            bal[e.u] += x; bal[e.v] -= x;
            self(self, id + 1, cost + x * e.cost);
            bal[e.u] -= x; bal[e.v] += x;
        }
    };
    dfs(dfs, 0, 0);
    return ans;
}

void check(LRCostFlow &flow) {
    auto expected = brute(flow);
    for (int rep = 0; rep < 2; ++ rep) {
        auto [ok, ans] = flow.solve();
        assert(ok == bool(expected));
        if (! ok) { assert(flow.f.empty()); continue; }
        assert(ans == *expected);
        assert(flow.f.size() == flow.edges.size());
        V < ll > bal(flow.n + 1);
        ll sum = 0;
        for (int id = 0; id < int(flow.edges.size()); ++ id) {
            auto &e = flow.edges[id]; ll x = flow.f[id];
            assert(e.lo <= x && x <= e.hi);
            bal[e.u] += x; bal[e.v] -= x; sum += x * e.cost;
        }
        assert(bal == flow.supply && sum == ans);
    }
}

void test_random() {
    mt19937 rng(20261011);
    LRCostFlow flow;
    for (int test = 0; test < 25000; ++ test) {
        int n = int(rng() % 5) + 1, m = rng() % 9;
        flow.init(n);
        for (int id = 0; id < m; ++ id) {
            int u = int(rng() % n) + 1, v = int(rng() % n) + 1;
            ll lo = rng() % 3, hi = lo + rng() % 3;
            ll w = ll(rng() % 13) - 6;
            assert(flow.add_edge(u, v, lo, hi, w) == id);
        }
        check(flow);
        if (test & 1) {
            // 用合法流构造一定可行的供需。
            for (auto &e : flow.edges) {
                ll x = e.lo + rng() % (e.hi - e.lo + 1);
                flow.add_supply(e.u, x); flow.add_supply(e.v, -x);
            }
        } else {
            for (int u = 2; u <= n; ++ u) {
                ll b = ll(rng() % 9) - 4;
                flow.add_supply(u, b); flow.add_supply(1, -b);
            }
        }
        check(flow);
        flow.add_supply(1, 1); check(flow);
        flow.add_supply(1, -1); check(flow);
    }
}

void test_regressions() {
    LRCostFlow flow(4);
    flow.add_edge(1, 2, 0, 1, 5);
    flow.add_edge(3, 4, 0, 2, -7); flow.add_edge(4, 3, 0, 2, 1);
    flow.add_supply(1, 1); flow.add_supply(2, -1);
    check(flow); assert(flow.solve() == make_pair(true, -7ll));
    assert(flow.f == V < ll > ({1, 2, 2}));
    flow.init(2);
    flow.add_edge(1, 2, 2, 2, -10);
    assert(! flow.solve().first && flow.f.empty());
    flow.add_edge(2, 1, 0, 3, 3);
    check(flow); assert(flow.solve().second == -14);
    flow.init(1);
    flow.add_edge(1, 1, 1, 3, -8); flow.add_edge(1, 1, 2, 4, 6);
    flow.add_edge(1, 1, 1, 3, 0);
    check(flow); assert(flow.solve().second == -12);
    flow.init(2);
    flow.add_edge(1, 2, 0, 3, -7); check(flow);
    assert(flow.solve().second == 0 && flow.f[0] == 0);
    flow.add_supply(1, 3); flow.add_supply(2, -3); check(flow);
    assert(flow.solve().second == -21);
    flow.init(0); check(flow);
}

void test_large_values() {
    LRCostFlow flow(2);
    const ll big = LLONG_MAX / 16;
    flow.add_edge(1, 2, 0, big, -1);
    flow.add_edge(2, 1, 0, big, 0);
    assert(flow.solve() == make_pair(true, -big));
    assert(flow.f == V < ll > ({big, big}));
    flow.init(3);
    const ll w = LLONG_MAX / 32;
    flow.add_edge(1, 2, 1, 2, -w);
    flow.add_edge(2, 3, 1, 2, w - 1);
    flow.add_edge(3, 1, 0, 2, 0);
    check(flow); assert(flow.solve().second == -2);
    flow.init(2);
    flow.add_edge(1, 2, 0, big, 1);
    flow.add_supply(1, big / 3); flow.add_supply(2, -big / 3);
    assert(flow.solve() == make_pair(true, big / 3));
    assert(flow.f[0] == big / 3);
}

int main() {
    test_regressions(); test_large_values(); test_random();
    return 0;
}
