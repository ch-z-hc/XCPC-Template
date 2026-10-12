#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 点分治：统计距离 <= k 的无序点对，不包含同一个点组成的点对。
// 连通树，顶点 1-based，边权非负；距离与答案须能用 ll 表示。
// 每次 query 为 O(n log^2 n)，空间 O(n)，可以重复调用。
// 找重心、收集距离和分治均递归；收集距离时递归深度最坏 O(n)。

struct Centroid {
    int n = 0;
    V < V < pair < int, ll > > > g;
    V < int > siz;
    V < char > vis;

    Centroid(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {}); siz.assign(n + 1, 0); vis.assign(n + 1, 0);
    }

    void add_edge(int u, int v, ll w = 1) {
        g[u].push_back({v, w}); g[v].push_back({u, w});
    }

    int dfs_size(int u, int fa) {
        siz[u] = 1;
        for (auto [v, w] : g[u])
            if (v != fa && ! vis[v]) siz[u] += dfs_size(v, u);
        return siz[u];
    }

    int find(int u, int fa, int total) {
        for (auto [v, w] : g[u])
            if (v != fa && ! vis[v] && siz[v] > total / 2)
                return find(v, u, total);
        return u;
    }

    void collect(int u, int fa, ll d, ll k, V < ll > &a) {
        if (d > k) return;
        a.push_back(d);
        for (auto [v, w] : g[u])
            if (v != fa && ! vis[v] && w <= k - d) collect(v, u, d + w, k, a);
    }

    ll count(V < ll > &a, ll k) {
        sort(a.begin(), a.end());
        ll ans = 0;
        int l = 0, r = int(a.size()) - 1;
        while (l < r) {
            if (a[l] <= k - a[r]) ans += r - l, l ++;
            else r --;
        }
        return ans;
    }

    ll solve(int s, ll k) {
        int u = find(s, 0, dfs_size(s, 0));
        vis[u] = 1;
        V < ll > all{0};
        ll ans = 0;
        for (auto [v, w] : g[u]) if (! vis[v]) {
            V < ll > a;
            collect(v, u, w, k, a);
            ans -= count(a, k);
            all.insert(all.end(), a.begin(), a.end());
        }
        ans += count(all, k);
        for (auto [v, w] : g[u]) if (! vis[v]) ans += solve(v, k);
        return ans;
    }

    ll query(ll k) {
        fill(vis.begin(), vis.end(), 0);
        return n && k >= 0 ? solve(1, k) : 0;
    }
};

// 调用示例：Centroid tree(n); tree.add_edge(u, v, w); ll ans = tree.query(k);
