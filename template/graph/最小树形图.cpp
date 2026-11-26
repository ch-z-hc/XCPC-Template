/*
求从 root 向外覆盖全图的最小树形图权值，O(n(n + m))。允许负权、重边、自环。
边权差和总权须放入 ll，根无法到达所有点时失败。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct DMST {
    struct Edge {
        int u, v;
        ll w;
    };

    int n = 0;
    V < Edge > edges;

    DMST(int n = 0) { init(n); }

    void init(int n_) { n = n_; edges.clear(); }

    void add_edge(int u, int v, ll w) { edges.push_back({u, v, w}); }

    bool solve(int root, ll &ans) {
        int k = n;
        ans = 0;
        V < Edge > g = edges;
        while (1) {
            V < ll > in(k + 1);
            V < int > fa(k + 1), id(k + 1), vis(k + 1);
            for (auto e : g) if (e.u != e.v) {
                if (! fa[e.v] || e.w < in[e.v]) {
                    fa[e.v] = e.u;
                    in[e.v] = e.w;
                }
            }
            in[root] = 0;
            for (int i = 1; i <= k; ++ i) {
                if (i != root && ! fa[i]) return false;
                ans += in[i];
            }
            int cnt = 0;
            for (int i = 1; i <= k; ++ i) {
                int v = i;
                while (v != root && ! id[v] && ! vis[v]) {
                    vis[v] = i;
                    v = fa[v];
                }
                if (v == root || id[v] || vis[v] != i) continue;
                id[v] = ++ cnt;
                for (int u = fa[v]; u != v; u = fa[u]) id[u] = cnt;
            }
            if (! cnt) return true;
            for (int i = 1; i <= k; ++ i) if (! id[i]) id[i] = ++ cnt;
            for (auto &e : g) {
                if (id[e.u] != id[e.v]) e.w -= in[e.v];
                e.u = id[e.u]; e.v = id[e.v];
            }
            root = id[root]; k = cnt;
        }
    }
};

/*
DMST mst; mst.init(n); mst.add_edge(u, v, w);
ll ans; bool ok = mst.solve(root, ans);
*/
