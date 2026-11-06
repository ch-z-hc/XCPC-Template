#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 矩形 KM / Hungarian 最大权匹配；左右部编号均为 1-based，要求 n <= m。
// 匹配所有左点，右点可不匹配；允许负权、缺边，重边保留最大权。
// 复杂度 O(n^2 m)，空间 O(nm)；总权及求和过程须能用 ll 表示。
// 权值不能为 LLONG_MIN；距离、势能及其运算绝对值须 < LLONG_MAX / 4。
// solve 返回 {是否存在完备左匹配, 总权}；可重复调用。
// 成功后 ml[u]、mr[v] 为匹配点，0 表示未匹配；失败时二者全部为 0。
struct KM {
    int n, m;
    V < V < ll > > w;
    V < V < bool > > op;
    V < int > ml, mr;

    KM(int n = 0, int m = 0) { init(n, m); }

    void init(int n_, int m_) {
        n = n_; m = m_;
        w.assign(n + 1, V < ll > (m + 1));
        op.assign(n + 1, V < bool > (m + 1));
        ml.assign(n + 1, 0); mr.assign(m + 1, 0);
    }

    void add_edge(int u, int v, ll val) {
        if (! op[u][v] || val > w[u][v]) w[u][v] = val;
        op[u][v] = true;
    }

    pair < bool, ll > solve() {
        const ll inf = LLONG_MAX / 4;
        fill(ml.begin(), ml.end(), 0);
        fill(mr.begin(), mr.end(), 0);
        if (n > m) return {false, 0};
        V < ll > lx(n + 1), ly(m + 1);
        V < int > p(m + 1), pre(m + 1);
        for (int u = 1; u <= n; ++ u) {
            V < ll > dis(m + 1, inf);
            V < bool > vis(m + 1);
            p[0] = u;
            int v = 0;
            do {
                vis[v] = true;
                int x = p[v], to = 0;
                ll delta = inf;
                for (int j = 1; j <= m; ++ j) if (! vis[j]) {
                    if (op[x][j]) {
                        ll val = -w[x][j] - lx[x] - ly[j];
                        if (val < dis[j]) dis[j] = val, pre[j] = v;
                    }
                    if (dis[j] < delta) delta = dis[j], to = j;
                }
                if (delta == inf) return {false, 0};
                for (int j = 0; j <= m; ++ j) {
                    if (vis[j]) lx[p[j]] += delta, ly[j] -= delta;
                    else if (dis[j] != inf) dis[j] -= delta;
                }
                v = to;
            } while (p[v]);
            do {
                p[v] = p[pre[v]];
                v = pre[v];
            } while (v);
        }
        ll ans = 0;
        for (int v = 1; v <= m; ++ v) if (p[v]) {
            ml[p[v]] = v; mr[v] = p[v];
            ans += w[p[v]][v];
        }
        return {true, ans};
    }
};

// 用法：KM km(n, m);
// km.add_edge(u, v, w);
// auto [ok, ans] = km.solve();
// if (ok) { int v = km.ml[u]; }
