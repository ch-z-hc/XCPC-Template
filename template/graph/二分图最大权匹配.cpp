/*
求覆盖全部左点的最大权匹配，左侧 n <= 右侧 m，O(n²m)。只加允许的边，重边取最大权。
允许负权，边权不能取 LLONG_MIN；距离、势能及相关运算绝对值须小于 LLONG_MAX / 4，总权须放入 i64。
solve 成功时返回总权，ml、mr 记录搭档，未匹配为 0。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct KM {
    int n, m;
    std::vector<std::vector<i64>> w;
    std::vector<std::vector<bool>> op;
    std::vector<int> ml, mr;

    KM(int n = 0, int m = 0) { init(n, m); }

    void init(int n_, int m_) {
        n = n_;
        m = m_;
        w.assign(n + 1, std::vector<i64>(m + 1));
        op.assign(n + 1, std::vector<bool>(m + 1));
        ml.assign(n + 1, 0);
        mr.assign(m + 1, 0);
    }

    void add_edge(int u, int v, i64 val) {
        if (!op[u][v] || val > w[u][v]) w[u][v] = val;
        op[u][v] = true;
    }

    std::pair<bool, i64> solve() {
        const i64 inf = LLONG_MAX / 4;
        std::fill(ml.begin(), ml.end(), 0);
        std::fill(mr.begin(), mr.end(), 0);
        if (n > m) return {false, 0};
        std::vector<i64> lx(n + 1), ly(m + 1);
        std::vector<int> p(m + 1), pre(m + 1);
        for (int u = 1; u <= n; u ++) {
            std::vector<i64> dis(m + 1, inf);
            std::vector<bool> vis(m + 1);
            p[0] = u;
            int v = 0;
            do {
                vis[v] = true;
                int x = p[v], to = 0;
                i64 delta = inf;
                for (int j = 1; j <= m; j ++)
                    if (!vis[j]) {
                        if (op[x][j]) {
                            i64 val = -w[x][j] - lx[x] - ly[j];
                            if (val < dis[j]) dis[j] = val, pre[j] = v;
                        }
                        if (dis[j] < delta) delta = dis[j], to = j;
                    }
                if (delta == inf) return {false, 0};
                for (int j = 0; j <= m; j ++) {
                    if (vis[j])
                        lx[p[j]] += delta, ly[j] -= delta;
                    else if (dis[j] != inf)
                        dis[j] -= delta;
                }
                v = to;
            } while (p[v]);
            do {
                p[v] = p[pre[v]];
                v = pre[v];
            } while (v);
        }
        i64 ans = 0;
        for (int v = 1; v <= m; v ++)
            if (p[v]) {
                ml[p[v]] = v;
                mr[v] = p[v];
                ans += w[p[v]][v];
            }
        return {true, ans};
    }
};

/*
KM km; km.init(n, m);
km.add_edge(u, v, w);
auto [ok, ans] = km.solve();
*/
