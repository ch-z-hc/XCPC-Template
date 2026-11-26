/*
含负边的有向图全源最短路，遇负环失败，不可达记 inf。要求 n * max|w| < inf / 4。
时间 O(nm + n(n + m) log(n + m))。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct Johnson {
    static const i64 inf = 1ll << 62;
    struct Edge {
        int u, v;
        i64 w;
    };
    int n;
    std::vector<Edge> e;
    std::vector<std::vector<std::pair<int, i64>>> g;

    Johnson(int n = 0) { init(n); }

    void init(int n) {
        this->n = n;
        e.clear();
        g.assign(n + 1, {});
    }

    void add_edge(int u, int v, i64 w) {
        e.push_back({u, v, w});
        g[u].push_back({v, w});
    }

    bool solve(std::vector<std::vector<i64>> &ans) {
        std::vector<i64> h(n + 1);
        for (int i = 1; i <= n; i ++) {
            std::vector<i64> nh = h;
            bool op = 0;
            for (auto [u, v, w] : e)
                if (nh[v] > h[u] + w) {
                    nh[v] = h[u] + w;
                    op = 1;
                }
            h.swap(nh);
            if (!op) break;
            if (i == n) return false;
        }

        ans.assign(n + 1, std::vector<i64>(n + 1, inf));
        for (int s = 1; s <= n; s ++) {
            std::vector<i64> dis(n + 1, inf);
            std::priority_queue<std::pair<i64, int>, std::vector<std::pair<i64, int>>,
                                std::greater<std::pair<i64, int>>>
                q;
            dis[s] = 0;
            q.push({0, s});
            while (!q.empty()) {
                auto [d, u] = q.top();
                q.pop();
                if (d != dis[u]) continue;
                for (auto [v, w] : g[u]) {
                    i64 nd = d + w + h[u] - h[v];
                    if (dis[v] <= nd) continue;
                    dis[v] = nd;
                    q.push({nd, v});
                }
            }
            for (int v = 1; v <= n; v ++)
                if (dis[v] != inf) ans[s][v] = dis[v] - h[s] + h[v];
        }
        return true;
    }
};
const i64 Johnson::inf;

/*
Johnson g; g.init(n); g.add_edge(u, v, w);
std::vector<std::vector<i64>> dis; bool ok = g.solve(dis);
*/
