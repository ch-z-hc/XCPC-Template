#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
在编号 1..n 的连通树中，统计距离不超过 k 的无序点对。两点须不同，边权须非负。
可对同一棵树多次 query。距离和答案用 ll。每次 O(n log² n)，空间 O(n)，长链注意递归栈。
*/

struct Centroid {
    int n = 0;
    V < V < pair < int, ll > > > g;
    V < int > siz;
    V < char > vis;

    Centroid(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        siz.assign(n + 1, 0);
        vis.assign(n + 1, 0);
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

// Centroid tree(n); tree.add_edge(u, v, w); ll ans = tree.query(k);
