#include "../../template/ds/单侧最值线段树.cpp"

template < bool is_min >
void verify() {
    AddMinMaxSeg < is_min > st;
    ll id = is_min ? LLONG_MAX : LLONG_MIN, sign = is_min ? 1 : -1;
    st.init({});
    assert(st.query(1, 1) == id);
    st.init({0, sign * 50, sign * 500});
    st.modify(1, 2, sign * 2, sign * 20);
    st.add(1, 2, sign * -7);
    assert(st.query(2, 2) == sign * 13);
    st.modify(1, 2, sign * 9, sign * 100);
    assert(st.query(1, 1) == sign * 22);
    assert(st.query(2, 1) == id);

    mt19937 rng(320);
    auto pick = [&](int l, int r) {
        return uniform_int_distribution < int >(l, r)(rng);
    };
    for (int i = 0; i < 500; i ++) {
        int n = pick(1, 50);
        V < ll > a(n + 1);
        for (int j = 1; j <= n; j ++) a[j] = pick(-100, 100);
        st.init(a);
        for (int step = 0; step < 300; step ++) {
            int l = pick(1, n), r = pick(l, n);
            if (step % 19 < 6) l = 1, r = n;
            ll d = pick(-100, 100), v = pick(-100, 100);
            switch (pick(0, 3)) {
                case 0:
                    st.add(l, r, d);
                    for (int j = l; j <= r; j ++) a[j] += d;
                    break;
                case 1:
                    if constexpr (is_min) st.chmin(l, r, v);
                    else st.chmax(l, r, v);
                    for (int j = l; j <= r; j ++)
                        a[j] = is_min ? min(a[j], v) : max(a[j], v);
                    break;
                case 2:
                    st.modify(l, r, d, v);
                    for (int j = l; j <= r; j ++)
                        a[j] = is_min ? min(a[j] + d, v) : max(a[j] + d, v);
                    break;
                default: {
                    auto [mn, mx] = minmax_element(a.begin() + l, a.begin() + r + 1);
                    assert(st.query(l, r) == (is_min ? *mn : *mx));
                }
            }
            auto [mn, mx] = minmax_element(a.begin() + 1, a.end());
            assert(st.query(1, n) == (is_min ? *mn : *mx));
        }
        for (int j = 1; j <= n; j ++) assert(st.query(j, j) == a[j]);
    }
}

int main() {
    verify < true >();
    verify < false >();
    cout << "PASS: 300000 random add/min and add/max operations\n";
}
