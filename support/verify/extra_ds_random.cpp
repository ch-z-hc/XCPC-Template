#include <cassert>
#include "../../template/ds/Fenwick.cpp"
#include "../../template/ds/AffineSegTree.cpp"
#include "../../template/ds/Fenwick2D.cpp"
#include "../../template/ds/MergeSegTree.cpp"
#include "../../template/graph/DynamicConnectivity.cpp"
#include "../../template/ds/LCT.cpp"
#include "../../template/geometry/RectangleUnion.cpp"

mt19937_64 rng(2026101007);

int rnd(int l, int r) { return l + int(rng() % (r - l + 1)); }
ll norm(ll x, ll mod) { x %= mod; return x < 0 ? x + mod : x; }

void check_fenwick() {
    Fenwick bit;
    for (int tc = 0; tc < 2200; ++ tc) {
        int n = rnd(0, 35);
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = rnd(0, 20);
        bit.init(a);
        for (int step = 0; step < 40; ++ step) {
            if (n) {
                int x = rnd(1, n);
                ll v = rnd(-int(a[x]), 20);
                a[x] += v; bit.add(x, v);
            }
            ll sum = accumulate(a.begin(), a.end(), 0ll);
            ll k = rnd(-3, int(sum) + 3), pre = 0;
            int ans = n + 1;
            for (int i = 1; i <= n; ++ i) {
                pre += a[i];
                if (pre >= k) { ans = i; break; }
            }
            assert(bit.kth(k) == ans);
            assert(bit.query(n) == sum);
        }
        bit.init(n);
        assert(bit.query(n) == 0 && bit.kth(1) == n + 1);
        assert(bit.kth(0) == 1 && bit.kth(-3) == 1);
    }
    cout << "Fenwick kth: 2200 cases OK\n";
}

void check_affine() {
    AffineSegTree st;
    const ll mods[] = {1, 2, 17, 998244353, INT_MAX};
    for (int tc = 0; tc < 2200; ++ tc) {
        int n = rnd(0, 30);
        ll mod = mods[tc % 5];
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = rnd(-100, 100);
        if (n && tc % 3 == 0) a[1] = LLONG_MIN;
        st.init(a, mod);
        for (int i = 1; i <= n; ++ i) a[i] = norm(a[i], mod);
        for (int step = 0; step < 50; ++ step) {
            if (! n) {
                st.modify(1, 0, -3, -7);
                assert(st.query(1, 0) == 0);
                continue;
            }
            int l = rnd(1, n), r = rnd(1, n);
            if (l > r) swap(l, r);
            ll mul = rnd(-100, 100), add = rnd(-100, 100);
            if (step % 3 == 0) mul = 0;
            if (step % 13 == 0) mul = LLONG_MIN, add = LLONG_MAX;
            st.modify(l, r, mul, add);
            mul = norm(mul, mod); add = norm(add, mod);
            for (int i = l; i <= r; ++ i) a[i] = (a[i] * mul + add) % mod;
            l = rnd(1, n); r = rnd(1, n);
            if (l > r) swap(l, r);
            ll ans = 0;
            for (int i = l; i <= r; ++ i) ans = (ans + a[i]) % mod;
            assert(st.query(l, r) == ans);
            assert(st.query(r, l - 1) == 0);
        }
    }
    cout << "AffineSegTree: 2200 cases OK\n";
}

void check_fenwick2d() {
    Fenwick2D bit;
    const ll coord[] = {LLONG_MIN, LLONG_MIN + 1, -5, -1, 0, 1,
                        5, LLONG_MAX - 1, LLONG_MAX};
    auto get = [&]() { return coord[rnd(0, 8)]; };
    for (int tc = 0; tc < 2200; ++ tc) {
        int n = rnd(0, 25);
        V < pair < ll, ll > > points(n);
        for (auto &p : points) p = {get(), get()};
        bit.build(points);
        map < pair < ll, ll >, ll > a;
        for (int step = 0; step < 45; ++ step) {
            if (n) {
                auto [x, y] = points[rnd(0, n - 1)];
                ll v = rnd(-50, 50); a[{x, y}] += v; bit.add(x, y, v);
            }
            ll x1 = get(), x2 = get(), y1 = get(), y2 = get();
            if (step % 3) {
                if (x1 > x2) swap(x1, x2);
                if (y1 > y2) swap(y1, y2);
            }
            ll ans = 0;
            for (auto [p, v] : a)
                if (x1 <= p.first && p.first <= x2 &&
                    y1 <= p.second && p.second <= y2) ans += v;
            assert(bit.query(x1, y1, x2, y2) == ans);
            bool sx = step & 1, sy = step & 2;
            ans = 0;
            for (auto [p, v] : a)
                if ((sx ? p.first < x1 : p.first <= x1) &&
                    (sy ? p.second < y1 : p.second <= y1)) ans += v;
            assert(bit.prefix(x1, y1, sx, sy) == ans);
        }
        bit.build({});
        assert(bit.query(LLONG_MIN, LLONG_MIN, LLONG_MAX, LLONG_MAX) == 0);
    }
    cout << "Fenwick2D: 2200 cases OK\n";
}

void check_merge() {
    MergeSegTree st;
    for (int tc = 0; tc < 2200; ++ tc) {
        int n = rnd(1, 25);
        st.init(n);
        V < int > roots(6);
        V < V < ll > > a(6, V < ll > (n + 1));
        for (int step = 0; step < 65; ++ step) {
            int u = rnd(0, 5), v = (u + rnd(1, 5)) % 6;
            int l = rnd(1, n), r = rnd(1, n);
            if (l > r) swap(l, r);
            int type = rnd(0, 2);
            if (type == 0) {
                ll val = rnd(-int(a[u][l]), 20);
                st.add(roots[u], l, val); a[u][l] += val;
            } else if (type == 1) {
                st.merge(roots[u], roots[v]);
                for (int i = 1; i <= n; ++ i) a[u][i] += a[v][i];
                fill(a[v].begin(), a[v].end(), 0);
                assert(roots[v] == 0);
            } else {
                int p = st.split(roots[u], l, r);
                st.merge(roots[v], p);
                for (int i = l; i <= r; ++ i) a[v][i] += a[u][i], a[u][i] = 0;
                assert(p == 0);
            }
            for (int j = 0; j < 6; ++ j) {
                ll ans = 0, sum = 0;
                for (int i = 1; i <= n; ++ i) {
                    sum += a[j][i];
                    if (l <= i && i <= r) ans += a[j][i];
                }
                assert(st.query(roots[j], l, r) == ans);
                assert(st.query(roots[j], 1, n) == sum);
                ll k = rnd(-1, int(sum) + 2), pre = 0;
                int pos = -1;
                if (k >= 1 && k <= sum)
                    for (int i = 1; i <= n; ++ i) {
                        pre += a[j][i];
                        if (pre >= k) { pos = i; break; }
                    }
                assert(st.kth(roots[j], k) == pos);
            }
        }
        st.init(1);
        assert(st.query(0, 1, 1) == 0 && st.kth(0, 1) == -1);
    }
    cout << "MergeSegTree: 2200 cases OK\n";
}

V < int > path(const V < V < int > > &g, int s, int t) {
    V < int > p;
    auto dfs = [&](auto &&self, int u, int fa) -> bool {
        p.push_back(u);
        if (u == t) return true;
        for (int v = 1; v < int(g.size()); ++ v)
            if (v != fa && g[u][v] && self(self, v, u)) return true;
        p.pop_back();
        return false;
    };
    dfs(dfs, s, 0);
    return p;
}

bool connected(const V < V < int > > &g, int s, int t) {
    V < bool > vis(g.size());
    auto dfs = [&](auto &&self, int u) -> void {
        vis[u] = true;
        for (int v = 1; v < int(g.size()); ++ v)
            if (g[u][v] && ! vis[v]) self(self, v);
    };
    dfs(dfs, s);
    return vis[t];
}

void check_connectivity() {
    DynConn dc;
    dc.init(0); assert(dc.solve().empty());
    for (int tc = 0; tc < 2200; ++ tc) {
        int n = rnd(1, 14);
        dc.init(n);
        V < V < int > > g(n + 1, V < int > (n + 1));
        V < int > ans;
        for (int step = 0; step < 65; ++ step) {
            int u = rnd(1, n), v = rnd(1, n), type = rnd(0, 2);
            if (type == 0) {
                dc.add(u, v); g[u][v] ++;
                if (u != v) g[v][u] ++;
            } else if (type == 1 && g[u][v]) {
                dc.erase(v, u); g[u][v] --;
                if (u != v) g[v][u] --;
            } else {
                int id = dc.query(u, v);
                assert(id == int(ans.size()));
                ans.push_back(connected(g, u, v));
            }
            if (step % 17 == 0) assert(dc.solve() == ans);
        }
        assert(dc.solve() == ans && dc.solve() == ans);
    }
    dc.init(2);
    dc.add(1, 2); dc.add(2, 1); dc.erase(1, 2); dc.query(1, 2);
    dc.erase(2, 1); dc.query(1, 2);
    assert((dc.solve() == V < int > {1, 0}));
    cout << "DynamicConnectivity: 2200 cases OK\n";
}

void check_lct() {
    LCT tree;
    tree.init({0}); assert(tree.tr.size() == 1);
    for (int tc = 0; tc < 2200; ++ tc) {
        int n = rnd(1, 20);
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = rnd(-100, 100);
        tree.init(a);
        V < V < int > > g(n + 1, V < int > (n + 1));
        for (int step = 0; step < 85; ++ step) {
            int u = rnd(1, n), v = rnd(1, n), type = rnd(0, 4);
            bool same = ! path(g, u, v).empty();
            assert(tree.same(u, v) == same);
            if (type == 0) {
                bool ok = tree.link(u, v);
                assert(ok == ! same);
                if (ok) g[u][v] = g[v][u] = 1;
            } else if (type == 1) {
                bool ok = tree.cut(u, v);
                assert(ok == bool(g[u][v]));
                if (ok) g[u][v] = g[v][u] = 0;
            } else if (type == 2) {
                a[u] = rnd(-1000, 1000); tree.modify(u, a[u]);
            } else if (type == 3) {
                tree.make_root(u);
                int r = tree.find_root(v);
                assert(! path(g, r, v).empty());
            } else if (same) {
                ll sum = 0;
                for (int x : path(g, u, v)) sum += a[x];
                assert(tree.query(u, v) == sum);
                assert(tree.query(v, u) == sum);
            }
            assert(tree.query(u, u) == a[u]);
        }
    }
    cout << "LCT: 2200 cases OK\n";
}

pair < ll, ll > brute_rect(const V < array < ll, 4 > > &rects) {
    bool a[10][10]{};
    for (auto [x1, y1, x2, y2] : rects) {
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        for (int x = int(x1); x < x2; ++ x)
            for (int y = int(y1); y < y2; ++ y) a[x + 4][y + 4] = true;
    }
    ll area = 0, perimeter = 0;
    const int dx[] = {-1, 0, 1, 0}, dy[] = {0, -1, 0, 1};
    for (int x = 0; x < 10; ++ x) for (int y = 0; y < 10; ++ y) {
        if (! a[x][y]) continue;
        area ++;
        for (int i = 0; i < 4; ++ i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx < 0 || nx >= 10 || ny < 0 || ny >= 10 || ! a[nx][ny])
                perimeter ++;
        }
    }
    return {area, perimeter};
}

void check_rect() {
    RectUnion scan;
    const ll offsets[] = {0, LLONG_MIN + 100, LLONG_MAX - 100};
    for (int tc = 0; tc < 2500; ++ tc) {
        V < array < ll, 4 > > rects;
        int n = rnd(0, 18);
        for (int i = 0; i < n; ++ i) {
            if (i && rnd(0, 3) == 0) rects.push_back(rects[rnd(0, i - 1)]);
            else rects.push_back({rnd(-4, 4), rnd(-4, 4), rnd(-4, 4), rnd(-4, 4)});
        }
        auto ans = brute_rect(rects);
        assert(scan.solve(rects) == ans && scan.solve(rects) == ans);
        ll ox = offsets[tc % 3], oy = offsets[(tc / 3) % 3];
        for (auto &r : rects) r[0] += ox, r[2] += ox, r[1] += oy, r[3] += oy;
        assert(scan.solve(rects) == ans);
        assert((scan.solve({}) == pair < ll, ll > {0, 0}));
    }
    V < array < ll, 4 > > shared = {{0, 0, 2, 2}, {2, 0, 4, 2},
                                   {0, 0, 2, 2}, {4, 0, 4, 7}};
    assert((scan.solve(shared) == pair < ll, ll > {8, 12}));
    assert((scan.solve({{0, 0, 1000000000, 1000000000}}) ==
            pair < ll, ll > {1000000000000000000ll, 4000000000ll}));
    cout << "RectangleUnion: 2500 cases OK\n";
}

int main() {
    check_fenwick(); check_affine(); check_fenwick2d(); check_merge();
    check_connectivity(); check_lct(); check_rect();
    cout << "extra data structures random: OK\n";
}
