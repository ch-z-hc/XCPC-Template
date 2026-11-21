#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

/*
点编号 1..n。WDSU 的 merge(u, v, w) 约束 value[v] - value[u] = w。
XDSU 对应 value[u] ^ value[v] = w。merge 遇到矛盾返回 false。
query(u, v, ans) 连通时写入同方向的差值或异或值，否则返回 false。
操作均摊 O(α(n))，空间 O(n)。差值运算用 ll，注意累加范围。
*/

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
            fa[a] = b;
            dis[a] = -d;
            siz[b] += siz[a];
        } else {
            fa[b] = a;
            dis[b] = d;
            siz[a] += siz[b];
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
    V < ull > dis;

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

    bool merge(int u, int v, ull w) {
        int a = find(u), b = find(v);
        if (a == b) return (dis[u] ^ dis[v]) == w;
        ull d = dis[u] ^ dis[v] ^ w;
        if (siz[a] < siz[b]) swap(a, b);
        fa[b] = a;
        dis[b] = d;
        siz[a] += siz[b];
        return 1;
    }

    bool query(int u, int v, ull &ans) {
        if (find(u) != find(v)) return false;
        ans = dis[u] ^ dis[v];
        return true;
    }
};

/*
WDSU dsu(n); bool ok = dsu.merge(u, v, w);
ll d; bool connected = dsu.query(u, v, d);
*/
