// 统计树上距离 <= k 的无序异点对，单次 O(n log² n)。边权非负，距离和答案须放入 i64。

#include <bits/stdc++.h>

using i64 = long long;

struct Centroid {
    int n = 0;
    std::vector<std::vector<std::pair<int, i64>>> g;
    std::vector<int> siz;
    std::vector<char> vis;

    Centroid(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        siz.assign(n + 1, 0);
        vis.assign(n + 1, 0);
    }

    void add_edge(int u, int v, i64 w = 1) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    int dfs_size(int u, int fa) {
        siz[u] = 1;
        for (auto [v, w] : g[u])
            if (v != fa && !vis[v]) siz[u] += dfs_size(v, u);
        return siz[u];
    }

    int find(int u, int fa, int total) {
        for (auto [v, w] : g[u])
            if (v != fa && !vis[v] && siz[v] > total / 2) return find(v, u, total);
        return u;
    }

    void collect(int u, int fa, i64 d, i64 k, std::vector<i64> &a) {
        if (d > k) return;
        a.push_back(d);
        for (auto [v, w] : g[u])
            if (v != fa && !vis[v] && w <= k - d) collect(v, u, d + w, k, a);
    }

    i64 count(std::vector<i64> &a, i64 k) {
        std::sort(a.begin(), a.end());
        i64 ans = 0;
        int l = 0, r = int(a.size()) - 1;
        while (l < r) {
            if (a[l] <= k - a[r])
                ans += r - l, l ++;
            else
                r --;
        }
        return ans;
    }

    i64 solve(int s, i64 k) {
        int u = find(s, 0, dfs_size(s, 0));
        vis[u] = 1;
        std::vector<i64> all{0};
        i64 ans = 0;
        for (auto [v, w] : g[u])
            if (!vis[v]) {
                std::vector<i64> a;
                collect(v, u, w, k, a);
                ans -= count(a, k);
                all.insert(all.end(), a.begin(), a.end());
            }
        ans += count(all, k);
        for (auto [v, w] : g[u])
            if (!vis[v]) ans += solve(v, k);
        return ans;
    }

    i64 query(i64 k) {
        std::fill(vis.begin(), vis.end(), 0);
        return n && k >= 0 ? solve(1, k) : 0;
    }
};

// Centroid tree; tree.init(n); tree.add_edge(u, v, w); i64 ans = tree.query(k);
