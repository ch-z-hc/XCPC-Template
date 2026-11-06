#include <bits/stdc++.h>

#define V vector

using namespace std;

// 二分图最大匹配（Kuhn）；左右部顶点编号均为 1-based。
// 左右部大小 n、m，边数 E；复杂度 O(nE + n + m)，空间 O(n + m + E)。

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

// 用法：Kuhn match(n, m);
// match.add_edge(u, v);
// int ans = match.solve();
