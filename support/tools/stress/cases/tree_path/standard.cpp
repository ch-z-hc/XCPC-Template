#include "../../../../../template/graph/倍增最近公共祖先.cpp"

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, q, root; cin >> n >> q >> root; LCA tree(n);
    for (int i = 1; i < n; ++ i) {
        int u, v; cin >> u >> v; tree.add_edge(u, v);
    }
    tree.build(root);
    while (q --) {
        int u, v; cin >> u >> v; int p = tree.lca(u, v);
        cout << tree.dep[u] + tree.dep[v] - 2 * tree.dep[p] << '\n';
    }
}
