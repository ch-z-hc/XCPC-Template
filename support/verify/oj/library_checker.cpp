#include <bits/stdc++.h>
#include "../../../template/ds/仿射线段树.cpp"
#include "../../../template/ds/吉司机线段树.cpp"
#include "../../../template/ds/二维树状数组.cpp"
#include "../../../template/ds/动态树.cpp"
#include "../../../template/graph/一般图匹配.cpp"
#include "../../../template/graph/最小树形图.cpp"
#include "../../../template/math/二次剩余.cpp"
#include "../../../template/math/类欧几里得.cpp"
#include "../../../template/math/行列式与矩阵树定理.cpp"
#include "../../../template/math/多项式基本运算.cpp"
#include "../../../template/math/多项式多点求值与快速插值.cpp"
#include "../../../template/math/子集卷积.cpp"
#include "../../../template/math/矩阵求逆.cpp"
#include "../../../template/math/素性测试与整数分解.cpp"
#include "../../../template/math/原根.cpp"
#include "../../../template/geometry/点与向量.cpp"
#include "../../../template/geometry/平面最近点对.cpp"

// Library Checker 输入适配；只调用模板，不另写算法。
int main(int argc, char **argv) {
    assert(argc == 2);
    ios::sync_with_stdio(0); cin.tie(0);
    string mode = argv[1];
    if (mode == "range_affine_range_sum") {
        int n, q; cin >> n >> q;
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) cin >> a[i];
        AffineSegTree tree; tree.init(a, 998244353);
        while (q --) {
            int op, l, r; cin >> op >> l >> r;
            if (! op) {
                ll b, c; cin >> b >> c; tree.modify(l + 1, r, b, c);
            } else cout << tree.query(l + 1, r) << '\n';
        }
    } else if (mode == "range_chmin_chmax_add_range_sum") {
        int n, q; cin >> n >> q;
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) cin >> a[i];
        Beats tree; tree.init(a);
        while (q --) {
            int op, l, r; cin >> op >> l >> r;
            if (op == 3) cout << tree.query(l + 1, r) << '\n';
            else {
                ll x; cin >> x;
                if (op == 0) tree.chmin(l + 1, r, x);
                else if (op == 1) tree.chmax(l + 1, r, x);
                else tree.add(l + 1, r, x);
            }
        }
    } else if (mode == "point_add_rectangle_sum") {
        int n, q; cin >> n >> q;
        V < pair < ll, ll > > points;
        V < array < ll, 3 > > initial(n);
        V < array < ll, 5 > > ops;
        for (auto &p : initial) {
            cin >> p[0] >> p[1] >> p[2]; points.push_back({p[0], p[1]});
        }
        while (q --) {
            array < ll, 5 > op{}; cin >> op[0] >> op[1] >> op[2] >> op[3];
            if (op[0]) cin >> op[4];
            else points.push_back({op[1], op[2]});
            ops.push_back(op);
        }
        Fenwick2D bit; bit.build(points);
        for (auto p : initial) bit.add(p[0], p[1], p[2]);
        for (auto p : ops) {
            if (! p[0]) bit.add(p[1], p[2], p[3]);
            else cout << bit.query(p[1], p[2], p[3] - 1, p[4] - 1) << '\n';
        }
    } else if (mode == "dynamic_tree_vertex_add_path_sum") {
        int n, q; cin >> n >> q;
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) cin >> a[i];
        LCT tree; tree.init(a);
        for (int i = 1; i < n; ++ i) {
            int u, v; cin >> u >> v; assert(tree.link(u + 1, v + 1));
        }
        while (q --) {
            int op; cin >> op;
            if (! op) {
                int u, v, x, y; cin >> u >> v >> x >> y;
                assert(tree.cut(u + 1, v + 1)); assert(tree.link(x + 1, y + 1));
            } else if (op == 1) {
                int p; ll x; cin >> p >> x; ++ p;
                a[p] += x; tree.modify(p, a[p]);
            } else {
                int u, v; cin >> u >> v; cout << tree.query(u + 1, v + 1) << '\n';
            }
        }
    } else if (mode == "general_matching") {
        int n, m; cin >> n >> m; Blossom g; g.init(n);
        while (m --) { int u, v; cin >> u >> v; g.add_edge(u + 1, v + 1); }
        cout << g.solve() << '\n';
        for (int u = 1; u <= n; ++ u)
            if (g.match[u] > u) cout << u - 1 << ' ' << g.match[u] - 1 << '\n';
    } else if (mode == "directedmst") {
        int n, m, r; cin >> n >> m >> r; DMST g; g.init(n);
        while (m --) { int u, v; ll w; cin >> u >> v >> w; g.add_edge(u + 1, v + 1, w); }
        ll ans; bool ok = g.solve(r + 1, ans); assert(ok); cout << ans << '\n';
    } else if (mode == "sqrt_mod") {
        int t; cin >> t;
        while (t --) { ll a, p; cin >> a >> p; cout << sqrt_mod(a, p) << '\n'; }
    } else if (mode == "sum_of_floor_of_linear") {
        int t; cin >> t;
        while (t --) {
            ll n, m, a, b; cin >> n >> m >> a >> b;
            cout << floor_sum(n, m, a, b) << '\n';
        }
    } else if (mode == "primitive_root") {
        int q; cin >> q;
        while (q --) { ull p; cin >> p; cout << primitive_root(p) << '\n'; }
    } else if (mode == "inverse_matrix") {
        int n; cin >> n; V < V < ll > > a(n, V < ll > (n));
        for (auto &row : a) for (ll &x : row) cin >> x;
        V < V < ll > > b;
        if (! matrix_inv(a, 998244353, b)) cout << "-1\n";
        else for (const auto &row : b) {
            for (ll x : row) cout << x << ' ';
            cout << '\n';
        }
    } else if (mode == "matrix_det") {
        int n; cin >> n; V < V < ll > > a(n, V < ll > (n));
        for (auto &row : a) for (ll &x : row) cin >> x;
        cout << det(a, 998244353) << '\n';
    } else if (mode == "counting_spanning_tree_directed" ||
               mode == "counting_spanning_tree_undirected") {
        int n, m, r = 0; cin >> n >> m;
        bool directed = mode == "counting_spanning_tree_directed";
        if (directed) cin >> r;
        V < tuple < int, int, ll > > edges;
        while (m --) { int u, v; cin >> u >> v; edges.push_back({u + 1, v + 1, 1}); }
        cout << matrix_tree(n, edges, 998244353, r + 1, directed) << '\n';
    } else if (mode == "closest_pair") {
        int t; cin >> t;
        while (t --) {
            int n; cin >> n; V < P > a(n);
            for (auto &p : a) cin >> p.x >> p.y;
            ClosestPair cp; auto [u, v] = cp.build(a); cout << u << ' ' << v << '\n';
        }
    } else if (mode == "division_of_polynomials") {
        int n, m; cin >> n >> m; V < int > a(n), b(m);
        for (int &x : a) cin >> x;
        for (int &x : b) cin >> x;
        auto [q, r] = FPS::divmod(a, b); cout << q.size() << ' ' << r.size() << '\n';
        for (int x : q) cout << x << ' ';
        cout << '\n';
        for (int x : r) cout << x << ' ';
        cout << '\n';
    } else if (mode == "pow_of_formal_power_series" || mode == "sqrt_of_formal_power_series") {
        int n; unsigned long long k = 0; cin >> n;
        if (mode == "pow_of_formal_power_series") cin >> k;
        V < int > a(n); for (int &x : a) cin >> x;
        V < int > ans;
        bool ok = true;
        if (mode == "pow_of_formal_power_series") ans = FPS::pow(a, k, n);
        else ok = FPS::sqrt(a, n, ans);
        if (! ok) cout << "-1\n";
        else { for (int x : ans) cout << x << ' '; cout << '\n'; }
    } else if (mode == "subset_convolution") {
        int n; cin >> n; V < int > a(1 << n), b(1 << n);
        for (int &x : a) cin >> x;
        for (int &x : b) cin >> x;
        for (int x : subset_convolution(a, b, 998244353)) cout << x << ' ';
        cout << '\n';
    } else if (mode == "multipoint_evaluation") {
        int n, m; cin >> n >> m; V < int > a(n), x(m);
        for (int &v : a) cin >> v;
        for (int &v : x) cin >> v;
        Multipoint poly; poly.build(x);
        for (int v : poly.evaluate(a)) cout << v << ' ';
        cout << '\n';
    } else if (mode == "polynomial_interpolation") {
        int n; cin >> n; V < int > x(n), y(n);
        for (int &v : x) cin >> v;
        for (int &v : y) cin >> v;
        Multipoint poly; poly.build(x);
        for (int v : poly.interpolate(y)) cout << v << ' ';
        cout << '\n';
    } else assert(false);
}
