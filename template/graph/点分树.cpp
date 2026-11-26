/*
维护静态非负权树的点权，求指定距离内的点权和。建树 O(n log² n)，单点加和查询 O(log² n)。
重建后点权归零，距离和权值和须放入 i64，长链注意递归栈。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct CentroidTree {
    struct Fenwick {
        std::vector<i64> dis, sum;

        void build() {
            std::sort(dis.begin(), dis.end());
            dis.erase(std::unique(dis.begin(), dis.end()), dis.end());
            sum.assign(dis.size() + 1, 0);
        }

        void modify(i64 d, i64 v) {
            int x = std::lower_bound(dis.begin(), dis.end(), d) - dis.begin() + 1;
            for (; x < int(sum.size()); x += x & -x) sum[x] += v;
        }

        i64 query(i64 d) {
            int x = std::upper_bound(dis.begin(), dis.end(), d) - dis.begin();
            i64 ans = 0;
            for (; x; x -= x & -x) ans += sum[x];
            return ans;
        }
    };

    struct Path {
        int u, branch;
        i64 dis;
    };

    int n = 0;
    std::vector<std::vector<std::pair<int, i64>>> g;
    std::vector<int> siz;
    std::vector<char> vis;
    std::vector<Fenwick> all;
    std::vector<std::vector<Fenwick>> sub;
    std::vector<std::vector<Path>> path;

    CentroidTree(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        siz.clear();
        vis.clear();
        all.clear();
        sub.clear();
        path.clear();
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

    void collect(int u, int fa, i64 d, int root, int branch) {
        path[u].push_back({root, branch, d});
        all[root].dis.push_back(d);
        sub[root][branch].dis.push_back(d);
        for (auto [v, w] : g[u])
            if (v != fa && !vis[v]) collect(v, u, d + w, root, branch);
    }

    void divide(int s) {
        int u = find(s, 0, dfs_size(s, 0));
        vis[u] = 1;
        path[u].push_back({u, -1, 0});
        all[u].dis.push_back(0);
        for (auto [v, w] : g[u])
            if (!vis[v]) {
                int branch = sub[u].size();
                sub[u].push_back({});
                collect(v, u, w, u, branch);
                sub[u].back().build();
            }
        all[u].build();
        for (auto [v, w] : g[u])
            if (!vis[v]) divide(v);
    }

    void build() {
        siz.assign(n + 1, 0);
        vis.assign(n + 1, 0);
        all.assign(n + 1, {});
        sub.assign(n + 1, {});
        path.assign(n + 1, {});
        if (n) divide(1);
    }

    void modify(int u, i64 delta) {
        for (auto [root, branch, d] : path[u]) {
            all[root].modify(d, delta);
            if (branch != -1) sub[root][branch].modify(d, delta);
        }
    }

    i64 query(int u, i64 radius) {
        i64 ans = 0;
        for (auto [root, branch, d] : path[u])
            if (radius >= d) {
                ans += all[root].query(radius - d);
                if (branch != -1) ans -= sub[root][branch].query(radius - d);
            }
        return ans;
    }
};

/*
CentroidTree tree; tree.init(n);
for (auto [u, v, w] : edges) tree.add_edge(u, v, w);
tree.build(); tree.modify(u, delta); i64 ans = tree.query(u, radius);
*/
