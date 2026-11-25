#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
先复制「势能最短路费用流」。求满足边流量上下界和顶点净供给的最小费用，支持有界负环。
0 <= lo <= hi，cost 不能取 LLONG_MIN。Σhi + Σ|供需| 和 Σ(hi * |cost|) 均须 <= LLONG_MAX / 4。
单点累计供需须放入 ll。增广 A 次耗时 O(nm + A m log n)。
*/

struct LRCostFlow {
    struct Edge {
        int u, v;
        ll lo, hi, cost;
    };
    int n = 0;
    V < Edge > edges;
    V < ll > supply, f;

    LRCostFlow(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edges.clear();
        f.clear();
        supply.assign(n + 1, 0);
    }

    int add_edge(int u, int v, ll lo, ll hi, ll cost) {
        assert(0 <= lo && lo <= hi && cost != LLONG_MIN);
        edges.push_back({u, v, lo, hi, cost});
        return int(edges.size()) - 1;
    }

    void add_supply(int u, ll b) {
        if (b >= 0) assert(supply[u] <= LLONG_MAX - b);
        else assert(supply[u] >= LLONG_MIN - b);
        supply[u] += b;
    }

    pair < bool, ll > solve() {
        f.clear();
        const ll limit = LLONG_MAX / 4;
        ll sum = 0, cap = 0, cost = 0;
        for (int u = 1; u <= n; ++ u) {
            assert(supply[u] != LLONG_MIN);
            ll x = abs(supply[u]);
            assert(x <= limit - cap);
            cap += x; sum += supply[u];
        }
        for (auto &e : edges) {
            assert(e.hi <= limit - cap);
            cap += e.hi;
            ll x = abs(e.cost);
            if (x) assert(e.hi <= (limit - cost) / x);
            cost += e.hi * x;
        }
        if (sum) return {false, 0};

        int ss = n + 1, tt = n + 2;
        MCMF net(n + 2);
        V < ll > bal = supply;
        V < pair < int, int > > pos;
        for (auto &e : edges) {
            bool neg = e.cost < 0;
            ll x = neg ? e.hi : e.lo;
            bal[e.u] -= x; bal[e.v] += x;
            int u = neg ? e.v : e.u, v = neg ? e.u : e.v;
            pos.push_back({u, int(net.g[u].size())});
            net.add_edge(u, v, e.hi - e.lo, neg ? -e.cost : e.cost);
        }
        ll need = 0;
        for (int u = 1; u <= n; ++ u) {
            if (bal[u] > 0) {
                net.add_edge(ss, u, bal[u], 0); need += bal[u];
            } else if (bal[u] < 0) net.add_edge(u, tt, -bal[u], 0);
        }
        if (net.flow(ss, tt).first != need) return {false, 0};
        ll ans = 0;
        for (int id = 0; id < int(edges.size()); ++ id) {
            auto &e = edges[id]; auto [u, k] = pos[id];
            ll rest = net.g[u][k].cap;
            ll x = e.cost < 0 ? e.lo + rest : e.hi - rest;
            f.push_back(x); ans += x * e.cost;
        }
        return {true, ans};
    }
};

/*
LRCostFlow g; g.init(n); int id = g.add_edge(u, v, lo, hi, w);
g.add_supply(s, k); g.add_supply(t, -k);
auto [ok, cost] = g.solve(); // 有解时 g.f[id] 是这条边的流量
*/
