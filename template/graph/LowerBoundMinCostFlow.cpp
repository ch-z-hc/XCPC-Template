#include <bits/stdc++.h>
#include <cassert>

#define V vector

typedef long long ll;

using namespace std;

// 上下界最小费用 b-flow；先复制 MinCostFlow.cpp 中的 MCMF。
// 顶点 1-based，0 <= lo <= hi，允许自环、重边和有界负费用负环。
// add_supply(u, b) 累加 u 的净流出需求；负数为需求，默认全 0 为循环流。
// solve 返回 {是否可行, 最小费用}；成功后 f[id] 为原边流，失败时 f 为空。
// 可重复 solve；init 清空图和需求。固定 s -> t 流量 k：s 加 k，t 加 -k。
// sum(hi) + sum(abs(b))、sum(hi * abs(cost)) 均须 <= LLONG_MAX / 4。
// 单点累计 supply 须能用 ll 表示；cost != LLONG_MIN，无模数要求。
// 时间 O(nm + A m log n)，A 为增广次数；空间 O(n + m)。
// 先饱和负费用边，初始残量费用非负；故不遗漏与需求无关的负环。
struct LRCostFlow {
    typedef __int128_t i128;
    struct Edge { int u, v; ll lo, hi, cost; };
    int n = 0;
    V < Edge > edges;
    V < ll > supply, f;

    LRCostFlow(int n = 0) { init(n); }

    void init(int n_) {
        n = n_; edges.clear(); f.clear(); supply.assign(n + 1, 0);
    }

    int add_edge(int u, int v, ll lo, ll hi, ll cost) {
        assert(0 <= lo && lo <= hi && cost != LLONG_MIN);
        edges.push_back({u, v, lo, hi, cost});
        return int(edges.size()) - 1;
    }

    void add_supply(int u, ll b) {
        i128 val = i128(supply[u]) + b;
        assert(LLONG_MIN <= val && val <= LLONG_MAX);
        supply[u] = ll(val);
    }

    pair < bool, ll > solve() {
        f.clear();
        i128 sum = 0, cap = 0, cost = 0;
        for (int u = 1; u <= n; ++ u) {
            sum += supply[u];
            cap += supply[u] < 0 ? -i128(supply[u]) : supply[u];
        }
        for (auto &e : edges) {
            cap += e.hi;
            cost += i128(e.hi) * (e.cost < 0 ? -i128(e.cost) : e.cost);
        }
        assert(cap <= LLONG_MAX / 4 && cost <= LLONG_MAX / 4);
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
        i128 ans = 0;
        for (int id = 0; id < int(edges.size()); ++ id) {
            auto &e = edges[id]; auto [u, k] = pos[id];
            ll rest = net.g[u][k].cap;
            ll x = e.cost < 0 ? e.lo + rest : e.hi - rest;
            f.push_back(x); ans += i128(x) * e.cost;
        }
        return {true, ll(ans)};
    }
};

// 调用：LRCostFlow flow(n); int id = flow.add_edge(u, v, lo, hi, cost);
// auto [ok, cost] = flow.solve(); if (ok) { ll x = flow.f[id]; }
