#include "../../template/ds/区间加与截断线段树.cpp"

void check(ClampSeg &st, const V < ll > &a, int l, int r) {
    auto [mn, mx] = minmax_element(a.begin() + l, a.begin() + r + 1);
    auto [low, high] = st.query(l, r);
    assert(low == *mn && high == *mx);
}

int main() {
    ClampSeg st;
    const pair < ll, ll > empty{LLONG_MAX, LLONG_MIN};
    st.init({});
    st.add(1, 1, 1);
    assert(st.query(1, 1) == empty);
    st.init({0, LLONG_MIN, LLONG_MAX});
    check(st, {0, LLONG_MIN, LLONG_MAX}, 1, 2);
    st.modify(1, 2, 0, -10, 20);
    st.add(1, 2, -5);
    check(st, {0, -15, 15}, 1, 1);
    check(st, {0, -15, 15}, 2, 2);
    st.chmax(1, 2, LLONG_MAX);
    st.add(1, 2, -3);
    check(st, {0, LLONG_MAX - 3, LLONG_MAX - 3}, 1, 1);
    st.init({0, 2, LLONG_MAX});
    st.modify(1, 2, 3, -10, 20);
    st.add(1, 2, 4);
    check(st, {0, 9, 24}, 2, 2);

    st.init({0, -15, -10, -5, 0, 5});
    st.chmax(1, 5, -10);
    st.add(1, 5, 10);
    st.chmin(1, 5, 10);
    for (int i = 1; i <= 5; i ++) check(st, {0, 0, 0, 5, 10, 10}, i, i);

    st.init({0, 3, 1, 4, -7});
    st.add(1, 4, 2);
    st.chmin(1, 2, 4); st.chmax(2, 4, 5);
    st.modify(1, 4, -2, 3, 8);
    check(st, {0, 3, 3, 4, 3}, 1, 4);
    assert(st.query(3, 2) == empty);

    mt19937 rng(2811);
    auto pick = [&](int l, int r) {
        return uniform_int_distribution < int >(l, r)(rng);
    };
    for (int i = 0; i < 800; i ++) {
        int n = pick(1, 64);
        V < ll > a(n + 1);
        for (int j = 1; j <= n; j ++) a[j] = pick(-100, 100);
        st.init(a);
        for (int step = 0; step < 300; step ++) {
            int l = pick(1, n), r = pick(l, n);
            if (step % 19 < 7) l = 1, r = n;
            ll d = pick(-100, 100), lo = pick(-100, 100), hi = pick(lo, 100);
            switch (pick(0, 5)) {
                case 0:
                    st.add(l, r, d);
                    for (int j = l; j <= r; j ++) a[j] += d;
                    break;
                case 1:
                    st.chmin(l, r, hi);
                    for (int j = l; j <= r; j ++) a[j] = min(a[j], hi);
                    break;
                case 2:
                    st.chmax(l, r, lo);
                    for (int j = l; j <= r; j ++) a[j] = max(a[j], lo);
                    break;
                case 3:
                    st.modify(l, r, d, lo, hi);
                    for (int j = l; j <= r; j ++) a[j] = min(max(a[j] + d, lo), hi);
                    break;
                case 4:
                    st.modify(l, r, d, lo, lo);
                    fill(a.begin() + l, a.begin() + r + 1, lo);
                    break;
                default:
                    check(st, a, l, r);
            }
            check(st, a, 1, n);
        }
        for (int j = 1; j <= n; j ++) check(st, a, j, j);
    }
    cout << "PASS: 240000 random clamp operations, extreme bounds and rebuilds\n";
}
