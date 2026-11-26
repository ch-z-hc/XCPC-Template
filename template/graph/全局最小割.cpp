/*
无向图全局最小割，O(n³)。每条边只加一次，边权非负且总和小于 LLONG_MAX。
不连通时割权为 0，点数不足 2 时返回 {0, {}}。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct MinCut {
    int n;
    V < V < ll > > g;

    MinCut(int n = 0) { init(n); }

    void init(int n) {
        this->n = n;
        g.assign(n + 1, V < ll > (n + 1));
    }

    void add_edge(int u, int v, ll w) {
        if (u == v) return;
        g[u][v] += w;
        g[v][u] += w;
    }

    pair < ll, V < int > > solve() {
        if (n < 2) return {0, {}};
        auto a = g;
        V < int > id(n);
        iota(id.begin(), id.end(), 1);
        V < V < int > > ver(n + 1);
        for (int i = 1; i <= n; ++ i) ver[i] = {i};
        pair < ll, V < int > > ans = {LLONG_MAX, {}};

        while (id.size() > 1) {
            V < ll > dis(n + 1);
            V < bool > vis(n + 1);
            int pre = 0;
            for (int i = 0; i < int(id.size()); ++ i) {
                int u = 0;
                for (int v : id)
                    if (! vis[v] && (! u || dis[v] > dis[u])) u = v;
                if (i + 1 == int(id.size())) {
                    if (dis[u] < ans.first) ans = {dis[u], ver[u]};
                    for (int v : id) if (v != u && v != pre) {
                        a[pre][v] += a[u][v];
                        a[v][pre] = a[pre][v];
                    }
                    ver[pre].insert(ver[pre].end(),
                                    ver[u].begin(), ver[u].end());
                    id.erase(find(id.begin(), id.end(), u));
                    break;
                }
                vis[u] = 1;
                pre = u;
                for (int v : id) if (! vis[v]) dis[v] += a[u][v];
            }
        }
        return ans;
    }
};

/*
MinCut g; g.init(n); g.add_edge(u, v, w);
auto [ans, side] = g.solve();
*/
