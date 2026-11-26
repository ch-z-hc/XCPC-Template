// 精确求无向图的最大团、最大独立集，最坏 O(2^n n²)，适合小图。忽略自环和重边。

#include <bits/stdc++.h>

#define V vector

using namespace std;

struct MaxClique {
    int n = 0;
    bool rev = 0;
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

    bool adjacent(int u, int v) { return bool(g[u][v]) != rev; }

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
                for (int v : color[c]) {
                    if (adjacent(u, v)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) break;
                c ++;
            }
            if (c == int(color.size())) color.push_back({});
            color[c].push_back(u);
        }
        V < int > ord, bound;
        for (int c = 0; c < int(color.size()); ++ c)
            for (int u : color[c]) {
                ord.push_back(u);
                bound.push_back(c + 1);
            }
        for (int i = int(ord.size()) - 1; i >= 0; -- i) {
            if (int(cur.size()) + bound[i] <= int(ans.size())) return;
            int u = ord[i];
            V < int > next;
            for (int j = 0; j < i; ++ j) if (adjacent(u, ord[j])) next.push_back(ord[j]);
            cur.push_back(u);
            dfs(next);
            cur.pop_back();
        }
    }

    V < int > solve(bool rev_) {
        rev = rev_;
        cur.clear();
        ans.clear();
        V < int > p(n); iota(p.begin(), p.end(), 1);
        dfs(p);
        return ans;
    }

    V < int > clique() { return solve(0); }
    V < int > indep() { return solve(1); }
};

/*
MaxClique mc; mc.init(n); mc.add_edge(u, v);
V < int > clique = mc.clique(), iset = mc.indep();
*/
