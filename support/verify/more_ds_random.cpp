#include <cassert>
#include "../../template/ds/吉司机线段树.cpp"
#include "../../template/ds/可持久化数组.cpp"

void check_beats(Beats &tr, const V < ll > &a, int l, int r) {
    ll sum = 0, mn = a[l], mx = a[l];
    for (int i = l; i <= r; ++ i) {
        sum += a[i]; mn = min(mn, a[i]); mx = max(mx, a[i]);
    }
    assert(tr.query(l, r) == sum);
    assert(tr.query_min(l, r) == mn && tr.query_max(l, r) == mx);
}

void check_ranges(Beats &tr, const V < ll > &a) {
    for (int l = 1; l < int(a.size()); ++ l)
        for (int r = l; r < int(a.size()); ++ r) check_beats(tr, a, l, r);
}

void apply_beats(Beats &tr, V < ll > &a, int op, int l, int r, ll val) {
    if (op == 0) tr.add(l, r, val);
    if (op == 1) tr.chmin(l, r, val);
    if (op == 2) tr.chmax(l, r, val);
    for (int i = l; i <= r; ++ i) {
        if (op == 0) a[i] += val;
        if (op == 1) a[i] = min(a[i], val);
        if (op == 2) a[i] = max(a[i], val);
    }
    check_beats(tr, a, l, r);
    check_beats(tr, a, 1, int(a.size()) - 1);
}

void test_beats(mt19937 &rng) {
    Beats tr;
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 30) + 1;
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) {
            if (test % 4 == 0) a[i] = -7;
            else if (test % 4 == 1) a[i] = i % 2 ? -9 : 11;
            else a[i] = int(rng() % 201) - 100;
        }
        tr.init(a); check_ranges(tr, a);
        for (int step = 0; step < 100; ++ step) {
            int l = int(rng() % n) + 1, r = int(rng() % n) + 1;
            if (l > r) swap(l, r);
            if (step % 7 == 0) l = 1, r = n;
            if (step % 7 == 1) r = l;
            ll val = int(rng() % 201) - 100;
            apply_beats(tr, a, step % 3, l, r, val);
            l = int(rng() % n) + 1; r = int(rng() % n) + 1;
            if (l > r) swap(l, r);
            check_beats(tr, a, l, r);
        }
        check_ranges(tr, a);
        tr.init(V < ll > {0});
        tr.init(V < ll > {0, -123});
        assert(tr.query(1, 1) == -123);
    }
    const ll big = 1ll << 59;
    V < ll > a{0, -big, big, big - 1, -big + 1, 0};
    tr.init(a); check_ranges(tr, a);
    for (int step = 0; step < 50; ++ step) {
        apply_beats(tr, a, 1, 1, 5, big - 100 - step);
        apply_beats(tr, a, 0, 1, 5, -37);
        apply_beats(tr, a, 2, 2, 4, -big + 100 + step);
        apply_beats(tr, a, 0, 2, 4, 19);
        check_ranges(tr, a);
    }
}

void check_version(const PArray &tr, int root, const V < ll > &a) {
    for (int i = 1; i < int(a.size()); ++ i) assert(tr.query(root, i) == a[i]);
}

void test_persistent(mt19937 &rng) {
    PArray tr;
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 30) + 1;
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = int(rng() % 201) - 100;
        tr.init(a);
        int first = tr.root;
        V < int > roots{first};
        V < V < ll > > history{a};
        for (int step = 0; step < 50; ++ step) {
            int old = rng() % roots.size(), pos = int(rng() % n) + 1;
            ll val = int(rng() % 2001) - 1000;
            if (step % 11 == 0) val = numeric_limits < ll > ::min();
            if (step % 11 == 1) val = numeric_limits < ll > ::max();
            a = history[old]; a[pos] = val;
            int root = tr.modify(roots[old], pos, val);
            assert(tr.root == first);
            check_version(tr, roots[old], history[old]);
            check_version(tr, root, a);
            roots.push_back(root); history.push_back(a);
            int id = rng() % roots.size();
            check_version(tr, roots[id], history[id]);
        }
        for (int i = 0; i < int(roots.size()); ++ i)
            check_version(tr, roots[i], history[i]);
        tr.init(V < ll > {0});
        tr.init(V < ll > {0, numeric_limits < ll > ::min()});
        int root = tr.modify(tr.root, 1, numeric_limits < ll > ::max());
        assert(tr.query(tr.root, 1) == numeric_limits < ll > ::min());
        assert(tr.query(root, 1) == numeric_limits < ll > ::max());
    }
}

int main() {
    mt19937 rng(20261011);
    test_beats(rng); test_persistent(rng);
    return 0;
}
