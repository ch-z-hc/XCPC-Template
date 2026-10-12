#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 朱刘算法，求以 root 为根、边从根向外的最小树形图。
// 顶点编号 1-based，允许负权、重边与自环；无解返回 false。
// solve 成功返回 true，将最小权写入 ans；失败时不要使用 ans。
// 时间 O(n(n + m))，有解时为 O(nm)；空间 O(n + m)。
// 所有权值减法及答案累加须在 ll 范围内；solve 不修改原图。
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

// 调用示例：DMST mst(n); mst.add_edge(u, v, w);
// ll ans; bool ok = mst.solve(root, ans); // 成功时使用 ans。
