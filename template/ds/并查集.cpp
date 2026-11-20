#include <bits/stdc++.h>

#define V vector

using namespace std;

// 点编号 1..n。merge(u, v) 合并两个集合，原本就在同一集合时返回 false。
// size(u) 是 u 所在集合的大小，cnt 是当前集合数。init(n) 重置为 n 个单点。
// 初始化 O(n)，合并、查询均摊 O(α(n))。
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

// DSU dsu(n);
// bool merged = dsu.merge(u, v);
// int siz = dsu.size(u);
