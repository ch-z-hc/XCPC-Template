#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 带权并查集，顶点 1-based；差值版 WDSU、异或版 XDSU。
// merge(u, v, w) 添加 value[v] - value[u] = w 或 value[u] xor value[v] = w。
// 返回约束是否相容；query(u, v, ans) 成功时将上述关系写入 ans。
// query 连通时返回 true，不连通返回 false，此时不要使用 ans。
// 路径压缩 + 按大小合并，均摊 O(alpha(n))，空间 O(n)；差值运算须不溢出 ll。

struct WDSU {
    V < int > fa, siz;
    V < ll > dis;

    WDSU(int n = 0) { init(n); }
    void init(int n) {
        fa.resize(n + 1); iota(fa.begin(), fa.end(), 0);
        siz.assign(n + 1, 1); dis.assign(n + 1, 0);
    }

    int find(int u) {
        if (fa[u] == u) return u;
        int p = fa[u];
        fa[u] = find(p); dis[u] += dis[p];
        return fa[u];
    }

    bool merge(int u, int v, ll w) {
        int a = find(u), b = find(v);
        if (a == b) return dis[v] - dis[u] == w;
        ll d = w + dis[u] - dis[v];
        if (siz[a] < siz[b]) {
            fa[a] = b; dis[a] = -d; siz[b] += siz[a];
        } else {
            fa[b] = a; dis[b] = d; siz[a] += siz[b];
        }
        return 1;
    }

    bool query(int u, int v, ll &ans) {
        if (find(u) != find(v)) return false;
        ans = dis[v] - dis[u];
        return true;
    }
};

struct XDSU {
    V < int > fa, siz;
    V < unsigned long long > dis;

    XDSU(int n = 0) { init(n); }
    void init(int n) {
        fa.resize(n + 1); iota(fa.begin(), fa.end(), 0);
        siz.assign(n + 1, 1); dis.assign(n + 1, 0);
    }

    int find(int u) {
        if (fa[u] == u) return u;
        int p = fa[u];
        fa[u] = find(p); dis[u] ^= dis[p];
        return fa[u];
    }

    bool merge(int u, int v, unsigned long long w) {
        int a = find(u), b = find(v);
        if (a == b) return (dis[u] ^ dis[v]) == w;
        unsigned long long d = dis[u] ^ dis[v] ^ w;
        if (siz[a] < siz[b]) swap(a, b);
        fa[b] = a; dis[b] = d; siz[a] += siz[b];
        return 1;
    }

    bool query(int u, int v, unsigned long long &ans) {
        if (find(u) != find(v)) return false;
        ans = dis[u] ^ dis[v];
        return true;
    }
};

// 调用示例：WDSU dsu(n); bool ok = dsu.merge(u, v, w);
// ll d; bool connected = dsu.query(u, v, d);
