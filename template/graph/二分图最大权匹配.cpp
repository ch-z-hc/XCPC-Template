#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
左侧 1..n 的每个点都要匹配到右侧 1..m 的不同点，要求 n <= m。
只加允许匹配的边。权可为负，重边取最大权。solve 返回 {是否能匹配全部左点, 最大总权}。
成功后 ml、mr 存两侧的搭档，0 表示未匹配。失败时两表清零，可再次求解。
边权不能取 LLONG_MIN，总权及中间加法用 ll，距离、势能及其运算绝对值须小于 LLONG_MAX / 4。
时间 O(n²m)，空间 O(nm)。
*/
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

/*
KM km(n, m);
km.add_edge(u, v, w);
auto [ok, ans] = km.solve();
*/
