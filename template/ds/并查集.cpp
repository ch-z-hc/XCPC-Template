#include <bits/stdc++.h>

#define V vector

using namespace std;

struct DSU {
    int cnt = 0;
    V < int > fa, siz;

    DSU(int n = 0) { init(n); }

    void init(int n) {
        cnt = n;
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int u) {
        if (fa[u] == u) return u;
        return fa[u] = find(fa[u]);
    }

    bool merge(int u, int v) {
        u = find(u); v = find(v);
        if (u == v) return 0;
        if (siz[u] < siz[v]) swap(u, v);
        fa[v] = u;
        siz[u] += siz[v];
        cnt --;
        return 1;
    }

    bool same(int u, int v) { return find(u) == find(v); }
    int size(int u) { return siz[find(u)]; }
};

/*
DSU dsu(n);
bool merged = dsu.merge(u, v);
int siz = dsu.size(u);
*/
