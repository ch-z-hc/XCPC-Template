#include <bits/stdc++.h>

#define V vector

using namespace std;

// 一般无向图的精确最大团 / 最大独立集；顶点编号 1-based，自环忽略，重边合并。
// 贪心染色给出上界，递归分支限界；最坏时间 O(2^n n^2)，空间 O(n^2)。
// 适用于小规模图，实际耗时依赖图结构。
// max_clique / max_independent_set 返回一个最优顶点集合，可重复调用；init 清空图。
struct MaxClique {
    int n = 0;
    bool independent = 0;
    V < V < char > > g;
    V < int > cur, ans;

    MaxClique(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, V < char > (n + 1));
        cur.clear(); ans.clear();
    }

    void add_edge(int u, int v) {
        if (u != v) g[u][v] = g[v][u] = 1;
    }

    bool adjacent(int u, int v) { return bool(g[u][v]) != independent; }

    void dfs(const V < int > &p) {
        if (p.empty()) {
            if (cur.size() > ans.size()) ans = cur;
            return;
        }
        V < V < int > > color;
        for (int u : p) {
            int c = 0;
            while (c < int(color.size())) {
                bool ok = 1;
                for (int v : color[c]) if (adjacent(u, v)) { ok = 0; break; }
                if (ok) break;
                c ++;
            }
            if (c == int(color.size())) color.push_back({});
            color[c].push_back(u);
        }
        V < int > ord, bound;
        for (int c = 0; c < int(color.size()); ++ c)
            for (int u : color[c]) { ord.push_back(u); bound.push_back(c + 1); }
        for (int i = int(ord.size()) - 1; i >= 0; -- i) {
            if (int(cur.size()) + bound[i] <= int(ans.size())) return;
            int u = ord[i];
            V < int > next;
            for (int j = 0; j < i; ++ j) if (adjacent(u, ord[j])) next.push_back(ord[j]);
            cur.push_back(u); dfs(next); cur.pop_back();
        }
    }

    V < int > solve(bool independent_) {
        independent = independent_; cur.clear(); ans.clear();
        V < int > p(n); iota(p.begin(), p.end(), 1);
        dfs(p);
        return ans;
    }

    V < int > max_clique() { return solve(0); }
    V < int > max_independent_set() { return solve(1); }
};

// 用法：MaxClique mc(n); mc.add_edge(u, v);
// V < int > clique = mc.max_clique(), independent = mc.max_independent_set();
