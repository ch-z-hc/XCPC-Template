#include "../../../../../template/ds/SegTree.cpp"

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, q; cin >> n >> q;
    V < ll > a(n + 1);
    for (int i = 1; i <= n; ++ i) cin >> a[i];
    SegTree tree; tree.init(a);
    while (q --) {
        int op, l, r; cin >> op >> l >> r;
        if (! op) { ll v; cin >> v; tree.modify(l, r, v); }
        else cout << tree.query(l, r) << '\n';
    }
}
