#include "../../template/ds/SegTree.cpp"
#include "../../template/ds/IntervalSet.cpp"
#include "../../template/graph/FunctionalGraph.cpp"
#include "../../template/others/LIS.cpp"
#include "../../template/others/SG.cpp"
#include "../../template/string/Hash2D.cpp"

void check_segtree(mt19937 &rng) {
    SegTree tree;
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = int(rng() % 35);
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = int(rng() % 31) - 15;
        tree.init(a);
        if (! n) {
            tree.modify(1, 0, 5);
            assert(tree.query(1, 0) == 0 && tree.kth(1) == -1);
            assert(tree.first_ge(1, 0, 0) == -1 && tree.last_ge(1, 0, 0) == -1);
            continue;
        }
        for (int step = 0; step < 100; ++ step) {
            int l = 1 + int(rng() % n), r = 1 + int(rng() % n);
            if (l > r) swap(l, r);
            ll v = int(rng() % 31) - 15;
            if (rng() % 2) {
                tree.modify(l, r, v);
                for (int i = l; i <= r; ++ i) a[i] += v;
            } else {
                ll sum = 0; int first = -1, last = -1;
                for (int i = l; i <= r; ++ i) {
                    sum += a[i];
                    if (a[i] >= v) { if (first == -1) first = i; last = i; }
                }
                assert(tree.query(l, r) == sum);
                assert(tree.first_ge(l, r, v) == first && tree.last_ge(l, r, v) == last);
            }
        }
        for (int i = 1; i <= n; ++ i) a[i] = rng() % 10;
        tree.init(a);
        tree.modify(1, n, 2);
        for (int i = 1; i <= n; ++ i) a[i] += 2;
        ll sum = accumulate(a.begin(), a.end(), 0ll);
        assert(tree.kth(0) == -1 && tree.kth(sum + 1) == -1);
        for (ll k = 1; k <= sum; ++ k) {
            ll pre = 0; int pos = 0;
            while (pre < k) pre += a[++ pos];
            assert(tree.kth(k) == pos);
        }
    }
    tree.init({0, 0, 0, 4, 0, 3});
    assert(tree.kth(1) == 3 && tree.kth(4) == 3 && tree.kth(5) == 5);
    tree.init({0, LLONG_MIN, LLONG_MAX});
    assert(tree.query(1, 2) == -1 && tree.first_ge(1, 2, LLONG_MAX) == 2);
}

void check_intervals(mt19937 &rng) {
    IntervalSet seg;
    for (int rep = 0; rep < 2000; ++ rep) {
        seg.clear(); V < bool > a(81, false);
        for (int step = 0; step < 60; ++ step) {
            int l = int(rng() % 41) - 20, r = int(rng() % 41) - 20;
            bool add = rng() % 2;
            if (add) seg.insert(l, r); else seg.erase(l, r);
            for (int i = l; i < r; ++ i) a[i + 40] = add;
            ll end = LLONG_MIN;
            for (auto [x, y] : seg.s) { assert(x < y && end < x); end = y; }
            for (int x = -30; x <= 30; ++ x) {
                assert(seg.contains(x) == a[x + 40]);
                int y = x;
                while (a[y + 40]) ++ y;
                assert(seg.mex(x) == y);
            }
        }
    }
    seg.clear(); seg.insert(LLONG_MIN, 0); seg.insert(0, LLONG_MAX);
    assert(seg.s.size() == 1 && seg.contains(LLONG_MIN) && ! seg.contains(LLONG_MAX));
    assert(seg.mex(LLONG_MIN) == LLONG_MAX);
    seg.erase(-1, 1);
    assert(seg.s.size() == 2 && seg.mex(LLONG_MIN) == -1 && seg.contains(1));
    seg.erase(LLONG_MIN, LLONG_MAX); assert(seg.s.empty());
}

void check_functional(mt19937 &rng) {
    FunctionalGraph graph;
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = 1 + int(rng() % 30);
        V < int > to(n + 1);
        for (int u = 1; u <= n; ++ u) to[u] = 1 + int(rng() % n);
        graph.build(to);
        V < bool > in_ring(n + 1, false);
        for (int id = 0; id < int(graph.cycles.size()); ++ id) {
            const auto &ring = graph.cycles[id];
            for (int i = 0; i < int(ring.size()); ++ i) {
                int u = ring[i]; assert(! in_ring[u]); in_ring[u] = true;
                assert(to[u] == ring[(i + 1) % ring.size()]);
                assert(graph.bel[u] == id && graph.pos[u] == i && graph.dep[u] == 0);
            }
        }
        for (int u = 1; u <= n; ++ u) {
            V < int > at(n + 1, -1), path;
            int v = u;
            while (at[v] == -1) {
                at[v] = int(path.size()); path.push_back(v); v = to[v];
            }
            int start = at[v], len = int(path.size()) - start;
            for (int w = 1; w <= n; ++ w) assert(graph.distance(u, w) == at[w]);
            assert(graph.dep[u] == start && graph.entry[u] == path[start]);
            for (ull k : {0ull, ull(rng() % 100), 1ull << 63, ULLONG_MAX}) {
                ull pos = k < path.size() ? k : start + (k - start) % len;
                assert(graph.jump(u, k) == path[size_t(pos)]);
            }
        }
    }
    graph.build({0}); assert(graph.cycles.empty() && graph.n == 0);
    graph.build({0, 1}); assert(graph.jump(1, ULLONG_MAX) == 1);
}

void check_lis(mt19937 &rng) {
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = int(rng() % 40);
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = int(rng() % 11) - 5;
        if (n >= 2 && rep % 10 == 0) a[1] = LLONG_MIN, a[2] = LLONG_MAX;
        for (bool strict : {false, true}) {
            V < int > dp(n + 1, 1); int best = 0;
            for (int i = 1; i <= n; ++ i) {
                for (int j = 1; j < i; ++ j)
                    if (strict ? a[j] < a[i] : a[j] <= a[i]) dp[i] = max(dp[i], dp[j] + 1);
                best = max(best, dp[i]);
            }
            auto ids = lis(a, strict); assert(int(ids.size()) == best);
            for (int i = 0; i < int(ids.size()); ++ i) {
                assert(1 <= ids[i] && ids[i] <= n);
                if (i) {
                    assert(ids[i - 1] < ids[i]);
                    assert(strict ? a[ids[i - 1]] < a[ids[i]] : a[ids[i - 1]] <= a[ids[i]]);
                }
            }
        }
    }
    assert(lis(V < int > {0}).empty());
}

void check_sg(mt19937 &rng) {
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = 1 + int(rng() % 15);
        V < int > order(n); iota(order.begin(), order.end(), 1);
        shuffle(order.begin(), order.end(), rng);
        V < V < int > > g(n + 1);
        for (int i = 0; i < n; ++ i) for (int j = i + 1; j < n; ++ j)
            if (rng() % 3 == 0) g[order[i]].push_back(order[j]);
        V < int > sg; assert(grundy(g, sg));
        V < V < bool > > win(n + 1, V < bool > (n + 1));
        for (int i = n - 1; i >= 0; -- i) for (int j = n - 1; j >= 0; -- j) {
            int u = order[i], v = order[j]; bool value = false;
            for (int x : g[u]) value = value || ! win[x][v];
            for (int y : g[v]) value = value || ! win[u][y];
            win[u][v] = value;
            assert(bool(sg[u] ^ sg[v]) == value);
        }
        for (int u = 1; u <= n; ++ u) {
            set < int > values;
            for (int v : g[u]) values.insert(sg[v]);
            int mex = 0; while (values.count(mex)) ++ mex;
            assert(sg[u] == mex);
        }
        g[order.back()].push_back(order.back()); assert(! grundy(g, sg));
    }
    V < int > sg;
    assert(grundy({{}}, sg) && sg.size() == 1);
    assert(! grundy({{}, {2}, {1}, {}}, sg));
    assert((grundy({{}, {2, 2}, {}}, sg) && sg == V < int > {0, 1, 0}));
}

ll direct_hash(const V < string > &a, int x1, int y1, int x2, int y2, ll mod) {
    ll ans = 0;
    for (int i = x1; i <= x2; ++ i) {
        ll row = 0;
        for (int j = y1; j <= y2; ++ j)
            row = (row * Hash2D::Table::by + static_cast < unsigned char > (a[i][j]) + 1) % mod;
        ans = (ans * Hash2D::Table::bx + row) % mod;
    }
    return ans;
}

void check_hash(mt19937 &rng) {
    Hash2D hash, other;
    for (int rep = 0; rep < 2000; ++ rep) {
        int n = 1 + int(rng() % 10), m = 1 + int(rng() % 10);
        V < string > a(n, string(m, '\0'));
        for (auto &row : a) for (char &c : row) c = char(rng() % 256);
        hash.build(a);
        for (int q = 0; q < 30; ++ q) {
            int x1 = int(rng() % n), x2 = int(rng() % n);
            int y1 = int(rng() % m), y2 = int(rng() % m);
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            auto value = hash.query(x1, y1, x2, y2);
            assert(value.first == direct_hash(a, x1, y1, x2, y2, 1000000007));
            assert(value.second == direct_hash(a, x1, y1, x2, y2, 1000000009));
            V < string > b;
            for (int i = x1; i <= x2; ++ i) b.push_back(a[i].substr(y1, y2 - y1 + 1));
            other.build(b);
            assert(value == other.query(0, 0, x2 - x1, y2 - y1));
        }
    }
    hash.build({}); assert(hash.query(0, 0, -1, -1) == make_pair(0ll, 0ll));
    hash.build({"", ""}); assert(hash.query(0, 0, 1, -1) == make_pair(0ll, 0ll));
}

int main() {
    mt19937 rng(261011);
    check_segtree(rng); check_intervals(rng); check_functional(rng);
    check_lis(rng); check_sg(rng); check_hash(rng);
    cout << "PASS: segment search, intervals, functional graph, LIS, SG, 2D hash\n";
}
