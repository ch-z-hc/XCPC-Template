#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
左右两侧分别编号 1..n 和 1..m，add_edge(u, v) 从左点 u 连到右点 v。
solve() 返回最大匹配边数，ml、mr 存两侧的搭档，0 表示未匹配。
设有 E 条边，时间 O(nE + n + m)，空间 O(n + m + E)。
*/

struct Kuhn {
    int n = 0, m = 0;
    V < V < int > > g;
    V < int > ml, mr;

    Kuhn(int n_ = 0, int m_ = 0) { init(n_, m_); }

    void init(int n_, int m_) {
        n = n_; m = m_;
        g.assign(n + 1, {});
        ml.assign(n + 1, 0);
        mr.assign(m + 1, 0);
    }

    void add_edge(int u, int v) { g[u].push_back(v); }

    bool dfs(int u, V < int > &vis, int tim = 1) {
        for (int v : g[u]) if (vis[v] != tim) {
            vis[v] = tim;
            if (! mr[v] || dfs(mr[v], vis, tim)) {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
        }
        return false;
    }

    int solve() {
        fill(ml.begin(), ml.end(), 0);
        fill(mr.begin(), mr.end(), 0);
        int ans = 0;
        V < int > vis(m + 1, 0);
        for (int u = 1; u <= n; ++ u)
            ans += dfs(u, vis, u);
        return ans;
    }
};

/*
Kuhn match(n, m);
match.add_edge(u, v);
int ans = match.solve();
*/
