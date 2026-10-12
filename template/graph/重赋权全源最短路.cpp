#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 有向图全源最短路，允许负权、自环和重边；顶点 1-based。
// Bellman-Ford 检测全图负环，再以势能重标边权运行 n 次 Dijkstra。
// 时间 O(nm + n(n + m) log n)，含结果矩阵的空间 O(n^2 + m)。
// 要求 n * max|w| < inf / 4，保证负环检测、势能和重标距离安全。
// 有限距离的绝对值小于 inf；不可达为 inf，全图有负环返回 false。
// solve 成功返回 true，将距离矩阵写入 ans；失败时不要使用 ans。
// solve 不修改图，可重复调用；不使用模数。
struct Johnson {
    static const ll inf = 1ll << 62;
    struct Edge { int u, v; ll w; };
    int n;
    V < Edge > e;
    V < V < pair < int, ll > > > g;

    Johnson(int n = 0) { init(n); }

    void init(int n) {
        this->n = n;
        e.clear();
        g.assign(n + 1, {});
    }

    void add_edge(int u, int v, ll w) {
        e.push_back({u, v, w});
        g[u].push_back({v, w});
    }

    bool solve(V < V < ll > > &ans) {
        V < ll > h(n + 1);
        for (int i = 1; i <= n; ++ i) {
            V < ll > nh = h;
            bool op = 0;
            for (auto [u, v, w] : e) if (nh[v] > h[u] + w) {
                nh[v] = h[u] + w;
                op = 1;
            }
            h.swap(nh);
            if (! op) break;
            if (i == n) return false;
        }

        ans.assign(n + 1, V < ll > (n + 1, inf));
        for (int s = 1; s <= n; ++ s) {
            V < ll > dis(n + 1, inf);
            priority_queue < pair < ll, int >,
                V < pair < ll, int > >,
                greater < pair < ll, int > > > q;
            dis[s] = 0;
            q.push({0, s});
            while (! q.empty()) {
                auto [d, u] = q.top(); q.pop();
                if (d != dis[u]) continue;
                for (auto [v, w] : g[u]) {
                    ll nd = d + w + h[u] - h[v];
                    if (dis[v] <= nd) continue;
                    dis[v] = nd;
                    q.push({nd, v});
                }
            }
            for (int v = 1; v <= n; ++ v)
                if (dis[v] != inf) ans[s][v] = dis[v] - h[s] + h[v];
        }
        return true;
    }
};
const ll Johnson::inf;
// Johnson g(n); g.add_edge(u, v, w);
// V < V < ll > > dis; bool ok = g.solve(dis); // 成功时使用 dis[u][v]。
