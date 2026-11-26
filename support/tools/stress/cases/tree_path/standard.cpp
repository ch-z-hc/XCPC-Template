#include "../../../../../template/graph/倍增最近公共祖先.cpp"

int main() {
    std::ios::sync_with_stdio(0); std::cin.tie(0);
    int n, q, root; std::cin >> n >> q >> root; LCA tree(n);
    for (int i = 1; i < n; ++ i) {
        int u, v; std::cin >> u >> v; tree.add_edge(u, v);
    }
    tree.build(root);
    while (q --) {
        int u, v; std::cin >> u >> v; int p = tree.lca(u, v);
        std::cout << tree.dep[u] + tree.dep[v] - 2 * tree.dep[p] << '\n';
    }
}
