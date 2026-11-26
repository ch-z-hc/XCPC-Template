#include "../../../../../template/ds/线段树与树上二分.cpp"

int main() {
    std::ios::sync_with_stdio(0); std::cin.tie(0);
    int n, q; std::cin >> n >> q;
    std::vector<i64> a(n + 1);
    for (int i = 1; i <= n; ++ i) std::cin >> a[i];
    SegTree tree; tree.init(a);
    while (q --) {
        int op, l, r; std::cin >> op >> l >> r;
        if (! op) { i64 v; std::cin >> v; tree.modify(l, r, v); }
        else std::cout << tree.query(l, r) << '\n';
    }
}
