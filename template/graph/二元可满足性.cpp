/*
变量编号 1..n，二元布尔约束可满足性判定 O(n + m)。
add(u, x, v, y) 表示 a[u] == x 或 a[v] == y。solve 无解时返回 false。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

struct SAT {
    int n = 0, tim = 0, cnt = 0;
    V < V < int > > g;
    V < int > dfn, low, bel, stk, ans;
    V < char > vis;

    SAT(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(2 * n, {});
        ans.assign(n + 1, 0);
    }

    void add(int u, bool x, int v, bool y) {
        int a = 2 * (u - 1) + x, b = 2 * (v - 1) + y;
        g[a ^ 1].push_back(b);
        g[b ^ 1].push_back(a);
    }

    void force(int u, bool x) { add(u, x, u, x); }

    void dfs(int u) {
        dfn[u] = low[u] = ++ tim;
        stk.push_back(u);
        vis[u] = 1;
        for (int v : g[u]) {
            if (!dfn[v])
                dfs(v), low[u] = min(low[u], low[v]);
            else if (vis[v])
                low[u] = min(low[u], dfn[v]);
        }
        if (low[u] != dfn[u]) return;
        ++ cnt;
        while (true) {
            int v = stk.back();
            stk.pop_back();
            bel[v] = cnt;
            vis[v] = 0;
            if (v == u) break;
        }
    }

    bool solve() {
        tim = cnt = 0;
        stk.clear();
        dfn.assign(2 * n, 0);
        low.assign(2 * n, 0);
        bel.assign(2 * n, 0);
        vis.assign(2 * n, 0);
        ans.assign(n + 1, 0);
        for (int u = 0; u < 2 * n; u ++)
            if (!dfn[u]) dfs(u);
        for (int u = 1; u <= n; u ++) {
            int a = 2 * (u - 1);
            if (bel[a] == bel[a ^ 1]) {
                ans.clear();
                return 0;
            }
            ans[u] = bel[a] > bel[a ^ 1];
        }
        return 1;
    }
};

/*
SAT sat; sat.init(2);
sat.add(1, 1, 2, 0);
sat.force(2, 1);
if (sat.solve()) cout << sat.ans[1];
*/
