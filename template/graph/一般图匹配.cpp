#include <bits/stdc++.h>

#define V vector

using namespace std;

// Edmonds 带花树，求一般无向图最大匹配；顶点编号 1-based。
// 时间 O(n^3)，空间 O(n^2)；邻接矩阵自动合并重边，自环忽略。
// solve 返回匹配边数；match[u] 为匹配点，未匹配为 0。
// 每次 solve 从空匹配重新计算；init 清空图。
struct Blossom {
    int n = 0;
    V < V < char > > g;
    V < int > match, fa, base, vis, used, flower;

    Blossom(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, V < char > (n + 1));
        match.assign(n + 1, 0);
        fa.resize(n + 1); base.resize(n + 1);
        vis.resize(n + 1); used.resize(n + 1); flower.resize(n + 1);
    }

    void add_edge(int u, int v) {
        if (u != v) g[u][v] = g[v][u] = 1;
    }

    int lca(int u, int v) {
        fill(vis.begin(), vis.end(), 0);
        while (1) {
            u = base[u]; vis[u] = 1;
            if (! match[u]) break;
            u = fa[match[u]];
        }
        while (! vis[base[v]]) v = fa[match[base[v]]];
        return base[v];
    }

    void mark(int u, int b, int v) {
        while (base[u] != b) {
            flower[base[u]] = flower[base[match[u]]] = 1;
            fa[u] = v;
            v = match[u]; u = fa[v];
        }
    }

    bool bfs(int root) {
        fill(fa.begin(), fa.end(), 0);
        fill(used.begin(), used.end(), 0);
        iota(base.begin(), base.end(), 0);
        queue < int > q;
        q.push(root); used[root] = 1;
        while (! q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 1; v <= n; ++ v) {
                if (! g[u][v] || base[u] == base[v] || match[u] == v) continue;
                if (v == root || (match[v] && fa[match[v]])) {
                    int b = lca(u, v);
                    fill(flower.begin(), flower.end(), 0);
                    mark(u, b, v); mark(v, b, u);
                    for (int i = 1; i <= n; ++ i) if (flower[base[i]]) {
                        base[i] = b;
                        if (! used[i]) q.push(i), used[i] = 1;
                    }
                } else if (! fa[v]) {
                    fa[v] = u;
                    if (! match[v]) {
                        for (int x = v; x; ) {
                            int p = fa[x], y = match[p];
                            match[x] = p; match[p] = x;
                            x = y;
                        }
                        return 1;
                    }
                    int x = match[v];
                    used[x] = 1; q.push(x);
                }
            }
        }
        return 0;
    }

    int solve() {
        fill(match.begin(), match.end(), 0);
        int ans = 0;
        for (int i = 1; i <= n; ++ i) if (! match[i]) ans += bfs(i);
        return ans;
    }
};

// 用法：Blossom blossom(n); blossom.add_edge(u, v);
// int ans = blossom.solve(); 匹配边可取所有 u < blossom.match[u]。
