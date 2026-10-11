#include <cassert>
#include "../../template/others/CDQ.cpp"
#include "../../template/others/OverallBinary.cpp"

ll random_val(mt19937 &rng) {
    unsigned x = rng() % 12;
    if (x == 0) return LLONG_MIN;
    if (x == 1) return LLONG_MAX;
    return int(rng() % 11) - 5;
}

void test_cdq(mt19937 &rng) {
    for (int test = 0; test < 1800; ++ test) {
        int n = rng() % 65;
        V < array < ll, 3 > > a(n);
        for (int i = 0; i < n; ++ i) {
            for (ll &x : a[i]) x = random_val(rng);
            if (i && rng() % 4 == 0) a[i] = a[rng() % i];
        }
        V < ll > want(n);
        for (int i = 0; i < n; ++ i) for (int j = 0; j < n; ++ j) if (i != j)
            want[i] += a[j][0] <= a[i][0] && a[j][1] <= a[i][1]
                       && a[j][2] <= a[i][2];
        auto old = a;
        assert(dominance(a) == want && a == old);
        assert(dominance(a) == want);
    }
    V < array < ll, 3 > > a(100, {LLONG_MIN, LLONG_MAX, 0});
    assert(dominance(a) == V < ll > (100, 99));
    assert(dominance({}).empty());
}

void test_kth(mt19937 &rng) {
    KthOffline solver;
    for (int test = 0; test < 1600; ++ test) {
        int n = int(rng() % 30) + 1;
        V < ll > a(n + 1), want;
        for (int i = 1; i <= n; ++ i) a[i] = random_val(rng);
        solver.init(a);
        int m = rng() % 100;
        for (int i = 0; i < m; ++ i) {
            if (rng() % 2) {
                int p = int(rng() % n) + 1;
                ll val = random_val(rng);
                solver.modify(p, val); a[p] = val;
            } else {
                int l = int(rng() % n) + 1, r = int(rng() % n) + 1;
                if (l > r) swap(l, r);
                int k = int(rng() % (r - l + 1)) + 1;
                V < ll > b(a.begin() + l, a.begin() + r + 1);
                sort(b.begin(), b.end());
                assert(solver.query(l, r, k) == int(want.size()));
                want.push_back(b[k - 1]);
            }
            if (i % 17 == 0) assert(solver.solve() == want);
        }
        assert(solver.solve() == want);
        assert(solver.solve() == want);
    }
    solver.init({0});
    assert(solver.solve().empty());
    solver.init({0, 7, 7, 7});
    assert(solver.solve().empty());
    solver.modify(2, 7);
    solver.query(1, 3, 3); solver.query(2, 2, 1);
    assert(solver.solve() == (V < ll > {7, 7}));
    solver.modify(2, LLONG_MIN);
    solver.query(1, 3, 1);
    solver.modify(1, LLONG_MAX);
    solver.query(1, 3, 3);
    assert(solver.solve() == (V < ll > {7, 7, LLONG_MIN, LLONG_MAX}));
    assert(solver.solve() == (V < ll > {7, 7, LLONG_MIN, LLONG_MAX}));
}

int main() {
    mt19937 rng(20261013);
    test_cdq(rng);
    test_kth(rng);
    cout << "PASS: CDQ / overall binary search random and regressions\n";
}
