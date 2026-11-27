/*
维护点对的差值或异或约束，操作均摊 O(α(n))。差值累加须放入 ll。
merge(u, v, w) 约束 value[v] - value[u] 或 value[u] ^ value[v] 等于 w。
约束矛盾时 merge 返回 false，两点不连通时 query 返回 false。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

struct WDSU {
    V < int > fa, siz;
    V < ll > dis;

    WDSU(int n = 0) { init(n); }
    void init(int n) {
        fa.resize(n + 1);
        iota(fa.begin(), fa.end(), 0);
        siz.assign(n + 1, 1);
        dis.assign(n + 1, 0);
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
            fa[a] = b; dis[a] = -d;
            siz[b] += siz[a];
        } else {
            fa[b] = a; dis[b] = d;
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
        fa.resize(n + 1);
        iota(fa.begin(), fa.end(), 0);
        siz.assign(n + 1, 1);
        dis.assign(n + 1, 0);
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
        fa[b] = a; dis[b] = d;
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
WDSU d; d.init(3); bool ok = d.merge(1, 2, 7);
ll w; if (d.query(1, 2, w)) cout << w;
int root = d.find(1);
XDSU x; x.init(3); x.merge(1, 2, 5);
ull v; if (x.query(1, 2, v)) cout << v;
int xr = x.find(1);
*/
