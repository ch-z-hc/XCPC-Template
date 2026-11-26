#include <bits/stdc++.h>

struct DSU {
    int cnt = 0;
    std::vector<int> fa, siz;

    DSU(int n = 0) { init(n); }

    void init(int n) {
        cnt = n;
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        std::iota(fa.begin(), fa.end(), 0);
    }

    int find(int u) {
        if (fa[u] == u) return u;
        return fa[u] = find(fa[u]);
    }

    bool merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return 0;
        if (siz[u] < siz[v]) std::swap(u, v);
        fa[v] = u;
        siz[u] += siz[v];
        cnt --;
        return 1;
    }

    bool same(int u, int v) { return find(u) == find(v); }
    int size(int u) { return siz[find(u)]; }
};

/*
DSU dsu; dsu.init(3); bool merged = dsu.merge(1, 2);
int root = dsu.find(2), siz = dsu.size(1); bool same = dsu.same(1, 2);
*/
