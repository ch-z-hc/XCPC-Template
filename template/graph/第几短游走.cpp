/*
求非负权有向图从 s 到 t 最短的 k 条游走长度。按边序列区分，可重复经过终点，零权环也计数。
s == t 时含空游走，不足 k 条返回全部。边权及有效长度须小于 inf。
时间 O((n + m) log(n + m + 1) + k log(k + 1))，空间 O(m + n log(n + m + 1) + k)。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct KWalk {
    static const ll inf = 1ll << 62;
    struct Edge {
        int u, v;
        ll w;
    };
    struct Node {
        ll val = 0;
        int to = 0, l = 0, r = 0, dis = 0;
    };
    int n = 0;
    V < Edge > edges;
    V < V < int > > g, rg;
    V < Node > tr;

    KWalk(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edges.clear();
        tr.clear();
        g.assign(n + 1, {});
        rg.assign(n + 1, {});
    }

    void add_edge(int u, int v, ll w) {
        int id = int(edges.size());
        edges.push_back({u, v, w});
        g[u].push_back(id);
        rg[v].push_back(id);
    }

    int merge(int u, int v, bool copy) {
        if (!u || !v) return u | v;
        if (tr[u].val > tr[v].val) swap(u, v);
        if (copy) {
            tr.push_back(tr[u]);
            u = int(tr.size()) - 1;
        }
        tr[u].r = merge(tr[u].r, v, copy);
        if (tr[tr[u].l].dis < tr[tr[u].r].dis) swap(tr[u].l, tr[u].r);
        tr[u].dis = tr[tr[u].r].dis + 1;
        return u;
    }

    V < ll > query(int s, int t, int k) {
        V < ll > ans;
        if (k <= 0) return ans;
        V < ll > dis(n + 1, inf);
        V < int > edge(n + 1, -1), ord, root(n + 1);
        priority_queue < pair < ll, int >, V < pair < ll, int > >,
                            greater < pair < ll, int > > >
            q;
        dis[t] = 0;
        q.push({0, t});
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (d != dis[u]) continue;
            ord.push_back(u);
            for (int id : rg[u]) {
                auto e = edges[id];
                if (e.w < inf - d && dis[e.u] > d + e.w) {
                    dis[e.u] = d + e.w;
                    edge[e.u] = id;
                    q.push({dis[e.u], e.u});
                }
            }
        }
        if (dis[s] == inf) return ans;
        tr.assign(1, Node{});
        tr[0].dis = -1;
        for (int u : ord) {
            for (int id : g[u]) {
                auto e = edges[id];
                if (id == edge[u] || dis[e.v] == inf) continue;
                ll delta = e.w + dis[e.v] - dis[u];
                tr.push_back({delta, e.v, 0, 0, 0});
                root[u] = merge(root[u], int(tr.size()) - 1, 0);
            }
            if (edge[u] != -1) root[u] = merge(root[u], root[edges[edge[u]].v], 1);
        }
        ans.push_back(dis[s]);
        if (root[s] && tr[root[s]].val < inf - dis[s])
            q.push({dis[s] + tr[root[s]].val, root[s]});
        while (int(ans.size()) < k && !q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            ans.push_back(d);
            for (int v : {tr[u].l, tr[u].r})
                if (v) {
                    ll delta = tr[v].val - tr[u].val;
                    if (delta < inf - d) q.push({d + delta, v});
                }
            int v = root[tr[u].to];
            if (v && tr[v].val < inf - d) q.push({d + tr[v].val, v});
        }
        return ans;
    }
};
const ll KWalk::inf;

/*
KWalk g; g.init(n); g.add_edge(u, v, w);
auto lengths = g.query(s, t, k);
*/
