#include "../../template/ds/树状数组.cpp"
#include "../../template/ds/线段树与树上二分.cpp"
#include "../../template/ds/仿射线段树.cpp"
#include "../../template/ds/主席树.cpp"

void check_fenwick(Fenwick &bit, const V < ll > &a) {
    assert(bit.n == int(a.size()) - 1);
    ll sum = 0;
    for (int i = 1; i <= bit.n; ++ i) {
        sum += a[i];
        assert(bit.query(i) == sum && bit.query(i, i) == a[i]);
    }
}

void check_seg(SegTree &st, const V < ll > &a) {
    assert(st.n == int(a.size()) - 1);
    for (int l = 1; l <= st.n; ++ l) {
        ll sum = 0, mx = LLONG_MIN;
        for (int r = l; r <= st.n; ++ r) {
            sum += a[r]; mx = max(mx, a[r]);
            assert(st.query(l, r) == sum);
            int first = l, last = r;
            while (a[first] != mx) ++ first;
            while (a[last] != mx) -- last;
            assert(st.first_ge(l, r, mx) == first);
            assert(st.last_ge(l, r, mx) == last);
        }
    }
}

ll norm(ll x, ll mod) { x %= mod; return x < 0 ? x + mod : x; }

void check_affine(AffineSeg &st, const V < ll > &a, ll mod) {
    assert(st.n == int(a.size()) - 1 && st.mod == mod);
    for (int l = 1; l <= st.n; ++ l) {
        ll sum = 0;
        for (int r = l; r <= st.n; ++ r) {
            sum = (sum + norm(a[r], mod)) % mod;
            assert(st.query(l, r) == sum);
        }
    }
}

void check_pst(PST &st, const V < ll > &a) {
    assert(st.root.size() == a.size());
    for (int l = 1; l < int(a.size()); ++ l) {
        V < ll > sorted;
        for (int r = l; r < int(a.size()); ++ r) {
            sorted.push_back(a[r]); sort(sorted.begin(), sorted.end());
            for (int k = 1; k <= int(sorted.size()); ++ k)
                assert(st.kth(l, r, k) == sorted[k - 1]);
            for (ll x : V < ll > {LLONG_MIN, -1, 0, 1, a[r], LLONG_MAX}) {
                int cnt = 0;
                for (int i = l; i <= r; ++ i) cnt += a[i] <= x;
                assert(st.count(l, r, x) == cnt);
            }
        }
    }
}

int main(int argc, char **argv) {
    string only = argc > 1 ? argv[1] : "";
    mt19937 rng(2026110702);
    V < V < ll > > cases{{0, 7}, {0}, {0, 0, 0}, {0, -5, 2, -5, 7}};
    for (int t = 0; t < 200; ++ t) {
        V < ll > a(2 + rng() % 8);
        for (int i = 1; i < int(a.size()); ++ i) a[i] = int(rng() % 21) - 10;
        cases.push_back(a);
    }
    if (only.empty() || only == "fenwick") for (const auto &input : cases) {
        auto a = input;
        Fenwick bit(a); check_fenwick(bit, a);
        if (bit.n) bit.add(bit.n, 3);
        a = bit.tr; bit.init(bit.tr); check_fenwick(bit, a);
        if (bit.n) bit.add(bit.n, -2), a.back() -= 2;
        check_fenwick(bit, a);
        bit.init(input); check_fenwick(bit, input);
    }
    if (only.empty() || only == "seg") for (const auto &input : cases) {
        for (auto member : {&SegTree::sum, &SegTree::mx, &SegTree::lazy}) {
            SegTree st; st.init(input); check_seg(st, input);
            if (st.n) st.modify(1, st.n, 3);
            auto a = st.*member;
            st.init(st.*member); check_seg(st, a);
            st.modify(1, st.n, -2);
            for (int i = 1; i <= st.n; ++ i) a[i] -= 2;
            check_seg(st, a);
            st.init(input); check_seg(st, input);
        }
    }
    if (only.empty() || only == "affine") for (const auto &input : cases) {
        for (ll mod : {1ll, 11ll, ll(INT_MAX)})
            for (auto member : {&AffineSeg::sum, &AffineSeg::mul,
                                &AffineSeg::add}) {
                AffineSeg st; st.init(input, mod); check_affine(st, input, mod);
                if (st.n) st.modify(1, st.n, -2, 3);
                auto a = st.*member;
                ll next = mod == 11 ? INT_MAX : 11;
                st.init(st.*member, next); check_affine(st, a, next);
                st.modify(1, st.n, -2, 3);
                for (int i = 1; i <= st.n; ++ i) a[i] = norm(-2 * norm(a[i], next) + 3, next);
                check_affine(st, a, next);
                st.init(input, mod); check_affine(st, input, mod);
            }
    }
    if (only.empty() || only == "pst") {
        cases.insert(cases.begin(), {0, 1, 2});
        cases.push_back({0, LLONG_MIN, LLONG_MAX, 0, LLONG_MIN});
        for (const auto &input : cases) {
            PST st; st.build(input); check_pst(st, input);
            while (! st.val.empty()) {
                auto a = st.val;
                st.build(st.val); check_pst(st, a);
            }
            st.build(input); check_pst(st, input);
        }
    }
    cout << "PASS: internal-array rebuilds, naive range oracles and subsequent updates\n";
}
