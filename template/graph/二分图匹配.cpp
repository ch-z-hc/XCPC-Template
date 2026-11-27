/*
二分图最大匹配，从点数较少的一侧增广。输入始终按原左、右两侧编号。
两侧点数为 n、m，边数为 E，增广 O(min(n, m) E)，初始化及清空匹配 O(n + m)。
ml、mr 按原左、右两侧记录搭档，未匹配为 0。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

struct Kuhn {
    int n = 0, m = 0;
    V < V < int > > g;
    V < int > ml, mr;

    Kuhn(int n_ = 0, int m_ = 0) { init(n_, m_); }

    void init(int n_, int m_) {
        n = n_;
        m = m_;
        g.assign(n + 1, {});
        ml.assign(n + 1, 0); mr.assign(m + 1, 0);
    }

    void add_edge(int u, int v) { g[u].push_back(v); }

    bool dfs(int u, V < int > &vis, int tim = 1) {
        for (int v : g[u])
            if (vis[v] != tim) {
                vis[v] = tim;
                if (!mr[v] || dfs(mr[v], vis, tim)) {
                    ml[u] = v;
                    mr[v] = u;
                    return true;
                }
            }
        return false;
    }

    int solve() {
        if (n > m) {
            Kuhn rev(m, n);
            for (int u = 1; u <= n; u ++)
                for (int v : g[u]) rev.add_edge(v, u);
            int ans = rev.solve();
            ml.swap(rev.mr);
            mr.swap(rev.ml);
            return ans;
        }
        fill(ml.begin(), ml.end(), 0);
        fill(mr.begin(), mr.end(), 0);
        int ans = 0;
        V < int > vis(m + 1, 0);
        for (int u = 1; u <= n; u ++) ans += dfs(u, vis, u);
        return ans;
    }
};

/*
Kuhn match; match.init(4, 2); match.add_edge(3, 1);
int cnt = match.solve();
*/
