#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 无向图顶点编号 1..n。每条边只加一次，可有重边和自环。边权非负，总和须小于 LLONG_MAX。
// solve 返回 {最小割权值, 割的一侧顶点}，可重复调用。图不连通时割权为 0，点数不足 2 返回 {0, {}}。
// 时间 O(n³)，空间 O(n²)。
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

// MinCut g(n); g.add_edge(u, v, w); // 无向边只添加一次。
// auto [ans, side] = g.solve();
