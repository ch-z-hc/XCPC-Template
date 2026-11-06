#include <bits/stdc++.h>

#define V vector

using namespace std;

// 普通并查集；编号为 1..n，路径压缩配合按大小合并。
// 初始化 O(n)，单次操作摊销 O(alpha(n))，空间 O(n)。
// merge 返回是否成功合并；cnt 为连通块数，init 可重新初始化。
// siz 仅对根有效，集合大小用 size(u) 查询。
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

// 用法：DSU dsu(n);
// dsu.merge(u, v);
// bool op = dsu.same(u, v);
// int cnt = dsu.size(u);
