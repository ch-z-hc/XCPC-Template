#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 矩形 KM / Hungarian 最大权匹配；左右部编号均为 1-based，要求 n <= m。
// 匹配所有左点，右点可不匹配；允许负权、缺边，重边保留最大权。
// 复杂度 O(n^2 m)，空间 O(nm)；权值用 ll，最优匹配总权须能用 ll 表示。
// 中间距离与势能用 __int128_t，取负时不会在 LLONG_MIN 处溢出。
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
        typedef __int128_t i128;
        const i128 inf = i128(1) << 120;
        fill(ml.begin(), ml.end(), 0);
        fill(mr.begin(), mr.end(), 0);
        if (n > m) return {false, 0};
        V < i128 > lx(n + 1), ly(m + 1);
        V < int > p(m + 1), pre(m + 1);
        for (int u = 1; u <= n; ++ u) {
            V < i128 > dis(m + 1, inf);
            V < bool > vis(m + 1);
            p[0] = u;
            int v = 0;
            do {
                vis[v] = true;
                int x = p[v], to = 0;
                i128 delta = inf;
                for (int j = 1; j <= m; ++ j) if (! vis[j]) {
                    if (op[x][j]) {
                        i128 val = -i128(w[x][j]) - lx[x] - ly[j];
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
        i128 ans = 0;
        for (int v = 1; v <= m; ++ v) if (p[v]) {
            ml[p[v]] = v; mr[v] = p[v];
            ans += w[p[v]][v];
        }
        return {true, ll(ans)};
    }
};

// 调用示例：
// KM km(n, m);
// km.add_edge(u, v, w);
// auto [ok, ans] = km.solve();
// if (ok) { int v = km.ml[u]; }
