#include <bits/stdc++.h>
#include <cassert>

#include "../../template/others/MoModify.cpp"
#include "../../template/others/MoTree.cpp"
#include "../../template/others/DPOpt.cpp"
#include "../../template/string/BitsetLCS.cpp"
#include "../../template/geometry/Point.cpp"
#include "../../template/geometry/Line.cpp"
#include "../../template/geometry/Polygon.cpp"
#include "../../template/geometry/ConvexHull.cpp"
#include "../../template/geometry/ClosestPair.cpp"
#include "../../template/geometry/ConvexQuery.cpp"

using namespace std;

mt19937 rng(20261011);

int rnd(int l, int r) {
    return uniform_int_distribution < int > (l, r)(rng);
}

V < int > mo_values, mo_count, mo_active, mo_answer;
int mo_distinct = 0;

void MoModify::add(int x) {
    assert(x >= 0 && x < int(mo_count.size()));
    if (! mo_count[x] ++) mo_distinct ++;
}
void MoModify::del(int x) {
    assert(x >= 0 && x < int(mo_count.size()) && mo_count[x] > 0);
    if (! -- mo_count[x]) mo_distinct --;
}
void MoModify::answer(int id) {
    assert(id >= 0 && id < int(mo_answer.size()) && mo_answer[id] == -1);
    mo_answer[id] = mo_distinct;
}
void MoTree::add(int u) {
    assert(u >= 1 && u <= n && ! mo_active[u]);
    mo_active[u] = 1;
    if (! mo_count[mo_values[u]] ++) mo_distinct ++;
}
void MoTree::del(int u) {
    assert(u >= 1 && u <= n && mo_active[u] && mo_count[mo_values[u]] > 0);
    mo_active[u] = 0;
    if (! -- mo_count[mo_values[u]]) mo_distinct --;
}
void MoTree::answer(int id) {
    assert(id >= 0 && id < int(mo_answer.size()) && mo_answer[id] == -1);
    mo_answer[id] = mo_distinct;
}

void check_mo_modify() {
    MoModify mo;
    mo.init({0});
    mo.solve();
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(1, 30);
        V < int > a(n + 1), truth;
        for (int i = 1; i <= n; ++ i) a[i] = rnd(0, 20);
        mo.init(a);
        int ops = rnd(0, 100);
        while (ops --) {
            if (rnd(0, 1)) {
                int p = rnd(1, n), x = rnd(0, 20);
                mo.modify(p, x); a[p] = x;
            } else {
                int l = rnd(1, n), r = rnd(l, n);
                int id = mo.query(l, r);
                assert(id == int(truth.size()));
                set < int > s(a.begin() + l, a.begin() + r + 1);
                truth.push_back(int(s.size()));
            }
        }
        for (int repeat = 0; repeat < 2; ++ repeat) {
            mo_count.assign(21, 0); mo_answer.assign(truth.size(), -1);
            mo_distinct = 0;
            mo.solve();
            assert(mo_answer == truth);
        }
    }
}

int brute_path(const V < V < int > > &g, const V < int > &a, int u, int v) {
    V < int > path;
    auto dfs = [&](auto &&self, int x, int fa) -> bool {
        if (x == v) { path.push_back(x); return true; }
        for (int y : g[x]) if (y != fa && self(self, y, x)) {
            path.push_back(x);
            return true;
        }
        return false;
    };
    assert(dfs(dfs, u, 0));
    set < int > vals;
    for (int x : path) vals.insert(a[x]);
    return int(vals.size());
}

void check_mo_tree() {
    MoTree mo;
    mo.build();
    mo.solve();
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(1, 30), root = rnd(1, n);
        mo.init(n);
        V < V < int > > g(n + 1);
        V < int > a(n + 1), truth;
        V < pair < int, int > > queries;
        for (int i = 1; i <= n; ++ i) a[i] = rnd(0, 20);
        for (int u = 2; u <= n; ++ u) {
            int v = rnd(1, u - 1);
            mo.add_edge(u, v);
            g[u].push_back(v); g[v].push_back(u);
        }
        mo.build(root);
        int q = rnd(0, 80);
        while (q --) {
            int u = rnd(1, n), v = rnd(1, n);
            queries.push_back({u, v});
            truth.push_back(brute_path(g, a, u, v));
            assert(mo.query(u, v) + 1 == int(truth.size()));
        }
        for (int repeat = 0; repeat < 3; ++ repeat) {
            if (repeat == 2) {
                mo.build(rnd(1, n));
                assert(mo.qs.empty());
                for (auto [u, v] : queries) mo.query(u, v);
            }
            mo_values = a; mo_count.assign(21, 0); mo_active.assign(n + 1, 0);
            mo_answer.assign(truth.size(), -1); mo_distinct = 0;
            mo.solve();
            assert(mo_answer == truth);
        }
    }
}

void check_dc_dp() {
    const ll inf = 1ll << 62;
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 24), layers = rnd(1, n + 4);
        V < ll > sum(n + 1), prev(n + 1, inf);
        for (int i = 1; i <= n; ++ i) sum[i] = sum[i - 1] + rnd(0, 10);
        prev[0] = 0;
        auto cost = [&](int l, int r) {
            ll s = sum[r] - sum[l - 1];
            return s * s;
        };
        while (layers --) {
            V < ll > truth(n + 1, inf);
            for (int i = 1; i <= n; ++ i)
                for (int j = 0; j < i; ++ j) if (prev[j] != inf)
                    truth[i] = min(truth[i], prev[j] + cost(j + 1, i));
            auto ans = dc_dp(prev, cost);
            assert(ans == truth);
            assert(dc_dp(prev, cost) == truth);
            prev = truth;
        }
    }
}

void check_knapsack() {
    const ll neg = -(1ll << 62);
    for (int t = 0; t < 2000; ++ t) {
        int capacity = rnd(0, 40), types = rnd(0, 8);
        V < ll > exact(capacity + 1, neg), upto(capacity + 1);
        exact[0] = 0;
        while (types --) {
            int w = rnd(1, capacity + 6), count = rnd(0, 6);
            ll value = rnd(-20, 20);
            auto check = [&](V < ll > &dp) {
                auto old = dp;
                V < ll > truth(capacity + 1, neg);
                for (int c = 0; c <= capacity; ++ c)
                    for (int k = 0; k <= count && k * w <= c; ++ k)
                        if (old[c - k * w] != neg)
                            truth[c] = max(truth[c], old[c - k * w] + k * value);
                bounded_knapsack(dp, w, value, count);
                assert(dp == truth);
            };
            check(exact); check(upto);
        }
    }
}

int brute_lcs(const string &s, const string &t) {
    V < int > prev(t.size() + 1), cur(t.size() + 1);
    for (unsigned char c : s) {
        for (int j = 1; j <= int(t.size()); ++ j) {
            cur[j] = max(cur[j - 1], prev[j]);
            if (c == (unsigned char)t[j - 1]) cur[j] = max(cur[j], prev[j - 1] + 1);
        }
        prev.swap(cur); cur[0] = 0;
    }
    return prev.back();
}

void check_lcs_pair(const string &s, const string &t) {
    int truth = brute_lcs(s, t);
    assert(bitset_lcs(s, t) == truth);
    assert(bitset_lcs(t, s) == truth);
    assert(bitset_lcs(s, t) == truth);
}

void check_lcs() {
    string bytes;
    for (int c = 0; c < 256; ++ c) bytes.push_back(char(c));
    string rev = bytes; reverse(rev.begin(), rev.end());
    check_lcs_pair(bytes, bytes);
    check_lcs_pair(bytes, rev);
    const V < int > lengths = {0, 1, 63, 64, 65, 127, 128, 129};
    for (int c = 0; c < 256; ++ c)
        for (int n : lengths) check_lcs_pair(string(n, char(c)), string(130, char(c)));
    for (int t = 0; t < 2000; ++ t) {
        int n = t % 2 ? rnd(0, 150) : lengths[rnd(0, int(lengths.size()) - 1)];
        int m = t % 2 ? rnd(0, 150) : lengths[rnd(0, int(lengths.size()) - 1)];
        int alphabet = t % 3 ? 8 : 256;
        string s(n, 0), u(m, 0);
        for (char &c : s) c = char(rnd(0, alphabet - 1));
        for (char &c : u) c = char(rnd(0, alphabet - 1));
        check_lcs_pair(s, u);
    }
}

void check_closest_case(ClosestPair &cp, const V < P > &points) {
    db truth = numeric_limits < db >::infinity();
    for (int i = 0; i < int(points.size()); ++ i)
        for (int j = i + 1; j < int(points.size()); ++ j) {
            P a = points[i], b = points[j];
            truth = min(truth, (a - b).abs2());
        }
    for (int repeat = 0; repeat < 2; ++ repeat) {
        auto [u, v] = cp.build(points);
        assert(cp.d2 == truth);
        if (points.size() < 2) {
            assert(u == -1 && v == -1);
            continue;
        }
        assert(u >= 0 && v >= 0 && u < int(points.size()));
        assert(v < int(points.size()) && u != v);
        P a = points[u], b = points[v];
        assert((a - b).abs2() == truth);
    }
}

void check_closest_pair() {
    ClosestPair cp;
    check_closest_case(cp, {});
    check_closest_case(cp, {{0, 0}});
    check_closest_case(cp, {{0, 0}, {0, 0}, {1, 1}});
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 60);
        V < P > a;
        for (int i = 0; i < n; ++ i) {
            db x = rnd(-40, 40), y = rnd(-40, 40);
            if (t % 4 == 0) y = 2 * x + 1;
            if (t % 4 == 1) x = 0;
            a.push_back({x, y});
        }
        shuffle(a.begin(), a.end(), rng);
        check_closest_case(cp, a);
    }
}

void check_convex_query() {
    ConvexQuery cq;
    cq.build({}); assert(cq.query({0, 0}) == 0);
    cq.build({{0, 0}}); assert(cq.query({0, 0}) == 1);
    assert(cq.query({1, 0}) == 0);
    cq.build({{0, 0}, {2, 0}, {1, 0}, {2, 0}});
    assert(cq.query({1, 0}) == 1 && cq.query({3, 0}) == 0);
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 40);
        V < P > a;
        for (int i = 0; i < n; ++ i) {
            db x = rnd(-20, 20), y = rnd(-20, 20);
            if (t % 5 == 0) y = 3 * x + 2;
            a.push_back({x, y});
        }
        shuffle(a.begin(), a.end(), rng);
        cq.build(a);
        auto hull = cq.a;
        for (int repeat = 0; repeat < 2; ++ repeat) {
            if (repeat) cq.build(a);
            for (int q = 0; q < 40; ++ q) {
                P p(rnd(-30, 30) / 2.0L, rnd(-30, 30) / 2.0L);
                assert(cq.query(p) == contain(hull, p));
            }
            for (int i = 0; i < int(hull.size()); ++ i) {
                assert(cq.query(hull[i]) == 1);
                P p = (hull[i] + hull[(i + 1) % hull.size()]) / 2;
                assert(cq.query(p) == 1);
            }
        }
    }
}

int main() {
    check_mo_modify();
    check_mo_tree();
    check_dc_dp();
    check_knapsack();
    check_lcs();
    check_closest_pair();
    check_convex_query();
    cout << "extra_offline_random OK: 7 algorithms x 2000 rounds + boundaries\n";
}
