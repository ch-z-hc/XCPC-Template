#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 先复制图论章的“最大流”中的 Dinic；无向非负容量图，顶点 1-based。
// 总容量须放入 ll；支持重边、自环和不连通图，可重复 build。
// t 为最小割树，路径最小边权等于两点最小割；query 同点约定返回 0。
// build 调用 n - 1 次最大流，另有 O(n^2 + n log n) 工作；query 为 O(log n)。
// 空间 O(n + m + n log n)，递归树遍历深度最坏 O(n)，无模数要求。
struct GomoryHu {
    struct Edge { int u, v; ll cap; };
    int n = 0, lg = 1;
    V < Edge > edges;
    V < V < pair < int, ll > > > t;
    V < int > fa, dep;
    V < ll > cut;
    V < V < int > > up;
    V < V < ll > > low;

    GomoryHu(int n = 0) { init(n); }
    void init(int n_) { n = n_; edges.clear(); }
    void add_edge(int u, int v, ll cap) { edges.push_back({u, v, cap}); }

    void dfs(int u, int p) {
        up[0][u] = p;
        for (int j = 1; j < lg; ++ j) {
            up[j][u] = up[j - 1][up[j - 1][u]];
            low[j][u] = min(low[j - 1][u], low[j - 1][up[j - 1][u]]);
        }
        for (auto [v, w] : t[u]) if (v != p) {
            dep[v] = dep[u] + 1; low[0][v] = w; dfs(v, u);
        }
    }

    void build() {
        fa.assign(n + 1, 1); fa[0] = 0;
        if (n) fa[1] = 0;
        cut.assign(n + 1, 0);
        for (int s = 2; s <= n; ++ s) {
            int p = fa[s];
            Dinic flow(n);
            for (auto [u, v, c] : edges) if (u != v) {
                flow.add_edge(u, v, c); flow.add_edge(v, u, c);
            }
            ll value = flow.flow(s, p);
            // 最后一次失败的 BFS 标出残量网络中源点可达的一侧。
            for (int u = s + 1; u <= n; ++ u)
                if (fa[u] == p && flow.dep[u] != -1) fa[u] = s;
            if (p != 1 && flow.dep[fa[p]] != -1) {
                fa[s] = fa[p]; fa[p] = s;
                cut[s] = cut[p]; cut[p] = value;
            } else cut[s] = value;
        }
        t.assign(n + 1, {});
        for (int u = 2; u <= n; ++ u) {
            t[u].push_back({fa[u], cut[u]}); t[fa[u]].push_back({u, cut[u]});
        }
        lg = 1; while ((1ll << lg) <= max(1, n)) lg ++;
        up.assign(lg, V < int > (n + 1));
        low.assign(lg, V < ll > (n + 1, LLONG_MAX)); dep.assign(n + 1, 0);
        if (n) dfs(1, 0);
    }

    ll query(int u, int v) const {
        if (u == v) return 0;
        ll ans = LLONG_MAX;
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int j = 0; j < lg; ++ j) if (d >> j & 1) {
            ans = min(ans, low[j][u]); u = up[j][u];
        }
        if (u == v) return ans;
        for (int j = lg - 1; j >= 0; -- j) if (up[j][u] != up[j][v]) {
            ans = min({ans, low[j][u], low[j][v]});
            u = up[j][u]; v = up[j][v];
        }
        return min({ans, low[0][u], low[0][v]});
    }
};

// 调用：GomoryHu tree(n); tree.add_edge(u, v, cap); tree.build();
// ll value = tree.query(s, t);
