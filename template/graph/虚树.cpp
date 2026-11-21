#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
先给编号 1..n 的无权连通树加边，再 build(root)。query(key) 返回虚树根，关键点可重复，空集返回 0。
nodes 是本次用到的关键点和 LCA，t[u] 存 {儿子, 原树距离}。下次 query 会先清空上次的节点。
建表时间和空间 O(n log n)，k 个关键点的查询 O(k log k + k log n)，长链注意递归栈。
*/

struct VTree {
    int n = 0, lg = 1, tim = 0;
    V < V < int > > g, fa;
    V < V < pair < int, int > > > t;
    V < int > in, out, dep, nodes;

    VTree(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        lg = 1;
        tim = 0;
        while ((1ll << lg) <= max(1, n)) lg ++;
        g.assign(n + 1, {}); t.assign(n + 1, {});
        fa.assign(lg, V < int > (n + 1));
        in.assign(n + 1, 0); out.assign(n + 1, 0);
        dep.assign(n + 1, 0); nodes.clear();
    }

    void add_edge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }

    void dfs(int u, int p) {
        in[u] = ++ tim; fa[0][u] = p;
        for (int j = 1; j < lg; ++ j) fa[j][u] = fa[j - 1][fa[j - 1][u]];
        for (int v : g[u]) if (v != p) {
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
        out[u] = tim;
    }

    void build(int root = 1) {
        for (int u : nodes) t[u].clear();
        nodes.clear();
        tim = 0;
        dep[root] = 0;
        dfs(root, 0);
    }

    bool ancestor(int u, int v) { return in[u] <= in[v] && out[v] <= out[u]; }

    int lca(int u, int v) {
        if (ancestor(u, v)) return u;
        if (ancestor(v, u)) return v;
        for (int j = lg - 1; j >= 0; -- j)
            if (fa[j][u] && ! ancestor(fa[j][u], v)) u = fa[j][u];
        return fa[0][u];
    }

    int query(V < int > key) {
        for (int u : nodes) t[u].clear();
        nodes.clear();
        if (key.empty()) return 0;
        auto cmp = [&](int u, int v) { return in[u] < in[v]; };
        sort(key.begin(), key.end(), cmp);
        key.erase(unique(key.begin(), key.end()), key.end());
        nodes = key;
        for (int i = 1; i < int(key.size()); ++ i)
            nodes.push_back(lca(key[i - 1], key[i]));
        sort(nodes.begin(), nodes.end(), cmp);
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
        for (int i = 1; i < int(nodes.size()); ++ i) {
            int v = nodes[i], u = lca(nodes[i - 1], v);
            t[u].push_back({v, dep[v] - dep[u]});
        }
        return nodes[0];
    }
};

/*
VTree tree(n); tree.add_edge(u, v); tree.build(root);
int rt = tree.query(key);
*/
