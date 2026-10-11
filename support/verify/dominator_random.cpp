#include <cassert>
#include "../../template/graph/Dominator.cpp"

V < int > reachable(const V < V < int > > &g, int root, int ban) {
    V < int > vis(g.size());
    if (root == ban) return vis;
    V < int > q{root}; vis[root] = 1;
    for (int i = 0; i < int(q.size()); ++ i)
        for (int v : g[q[i]]) if (v != ban && ! vis[v]) {
            vis[v] = 1; q.push_back(v);
        }
    return vis;
}

void check(Dominator &dt, int root) {
    int n = dt.n;
    V < int > reach = reachable(dt.g, root, 0);
    V < V < int > > dom(n + 1, V < int > (n + 1));
    for (int u = 1; u <= n; ++ u) {
        auto vis = reachable(dt.g, root, u);
        for (int v = 1; v <= n; ++ v) dom[u][v] = reach[v] && ! vis[v];
    }
    V < int > depth(n + 1), want(n + 1);
    for (int v = 1; v <= n; ++ v)
        for (int u = 1; u <= n; ++ u) depth[v] += dom[u][v];
    want[root] = root;
    for (int v = 1; v <= n; ++ v) if (v != root && reach[v])
        for (int u = 1; u <= n; ++ u)
            if (u != v && dom[u][v] && depth[u] > depth[want[v]]) want[v] = u;
    dt.build(root);
    assert(dt.idom == want);
    V < int > seen(n + 1);
    for (int u = 1; u <= n; ++ u) for (int v : dt.tree[u]) {
        assert(v != root && dt.idom[v] == u);
        seen[v] ++;
    }
    for (int v = 1; v <= n; ++ v) assert(seen[v] == (reach[v] && v != root));
}

int main() {
    mt19937 rng(20261011);
    for (int test = 0; test < 1500; ++ test) {
        int n = int(rng() % 9) + 1, m = rng() % 45;
        Dominator dt(n);
        for (int i = 0; i < m; ++ i)
            dt.add_edge(int(rng() % n) + 1, int(rng() % n) + 1);
        for (int root = 1; root <= n; ++ root) {
            check(dt, root); check(dt, root);
        }
        dt.add_edge(1, n); check(dt, 1);
        dt.init(2); dt.add_edge(1, 1); dt.add_edge(2, 1);
        check(dt, 1); check(dt, 2);
    }
    Dominator chain(2000);
    for (int i = 1; i < chain.n; ++ i) chain.add_edge(i, i + 1);
    chain.build(1);
    for (int i = 2; i <= chain.n; ++ i) assert(chain.idom[i] == i - 1);
    cout << "dominator random tests passed\n";
}
