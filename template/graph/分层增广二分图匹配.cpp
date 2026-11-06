#include <bits/stdc++.h>

#define V vector

using namespace std;

// Hopcroft-Karp 二分图最大匹配；左右部编号均为 1-based，允许重边。
// 边数为 E，复杂度 O(n + m + E sqrt(n + m))，空间 O(n + m + E)。
// BFS 构建最短增广路分层，DFS 递归增广；深链须留足栈空间。
// solve 返回匹配数，可重复调用；ml[u]、mr[v] 为匹配点，0 表示未匹配。
struct HK {
    int n, m;
    V < V < int > > g;
    V < int > ml, mr, dep, cur, vs;

    HK(int n = 0, int m = 0) { init(n, m); }

    void init(int n_, int m_) {
        n = n_; m = m_;
        g.assign(n + 1, {});
        ml.assign(n + 1, 0); mr.assign(m + 1, 0);
        dep.resize(n + 1); cur.resize(n + 1);
        vs.clear();
    }

    void add_edge(int u, int v) { g[u].push_back(v); }

    bool bfs() {
        queue < int > q;
        dep[0] = -1;
        for (int u : vs) {
            dep[u] = ml[u] ? -1 : 0;
            if (! ml[u]) q.push(u);
        }
        while (! q.empty()) {
            int u = q.front(); q.pop();
            if (dep[0] != -1 && dep[u] >= dep[0]) continue;
            for (int v : g[u]) if (dep[mr[v]] == -1) {
                dep[mr[v]] = dep[u] + 1;
                if (mr[v]) q.push(mr[v]);
            }
        }
        return dep[0] != -1;
    }

    bool dfs(int u) {
        if (! u) return true;
        for (int &i = cur[u]; i < int(g[u].size()); ++ i) {
            int v = g[u][i];
            if (dep[mr[v]] != dep[u] + 1 || ! dfs(mr[v])) continue;
            ml[u] = v; mr[v] = u;
            return true;
        }
        dep[u] = -1;
        return false;
    }

    int solve() {
        fill(ml.begin(), ml.end(), 0);
        fill(mr.begin(), mr.end(), 0);
        vs.clear();
        for (int u = 1; u <= n; ++ u) if (! g[u].empty()) vs.push_back(u);
        int ans = 0;
        while (bfs()) {
            for (int u : vs) cur[u] = 0;
            for (int u : vs) if (! ml[u]) ans += dfs(u);
        }
        return ans;
    }

    // 先 solve；O(n + m + E) 返回最小点覆盖 {左部顶点, 右部顶点}。
    // 两侧未被覆盖的顶点组成最大独立集。修改图后须重新 solve。
    pair < V < int >, V < int > > cover() {
        V < int > a(n + 1), b(m + 1), left, right;
        queue < int > q;
        for (int u = 1; u <= n; ++ u) if (! ml[u]) a[u] = 1, q.push(u);
        while (! q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) if (ml[u] != v && ! b[v]) {
                b[v] = 1;
                if (mr[v] && ! a[mr[v]]) a[mr[v]] = 1, q.push(mr[v]);
            }
        }
        for (int u = 1; u <= n; ++ u) if (! a[u]) left.push_back(u);
        for (int v = 1; v <= m; ++ v) if (b[v]) right.push_back(v);
        return {left, right};
    }
};

// 用法：HK hk(n, m);
// hk.add_edge(u, v);
// int ans = hk.solve();
