#include "../../template/ds/KDTree.cpp"
#include <cassert>

using Point = pair < ll, ll >;

optional < pair < ll, int > > brute_nearest(const V < Point > &ps,
                                           ll x, ll y, int exclude) {
    pair < ll, int > ans{LLONG_MAX, INT_MAX};
    for (int i = 0; i < (int)ps.size(); ++ i) {
        if (i == exclude) continue;
        ll dx = x - ps[i].first, dy = y - ps[i].second;
        ans = min(ans, pair{dx * dx + dy * dy, i});
    }
    if (ans.second == INT_MAX) return nullopt;
    return ans;
}

int brute_query(const V < Point > &ps, ll lx, ll ly, ll rx, ll ry) {
    int ans = 0;
    for (auto [x, y] : ps)
        ans += lx <= x && x <= rx && ly <= y && y <= ry;
    return ans;
}

int main() {
    KDTree kd;
    pair < ll, int > ans;
    assert(! kd.nearest(0, 0, ans)); assert(kd.query(-1, -1, 1, 1) == 0);
    kd.build({{0, 0}});
    assert((kd.nearest(0, 0, ans) && ans == pair < ll, int > {0, 0}));
    assert(! kd.nearest(0, 0, ans, 0));
    kd.build({{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 0}});
    assert((kd.nearest(0, 0, ans) && ans == pair < ll, int > {1, 0}));
    assert((kd.nearest(0, 0, ans, 0) && ans == pair < ll, int > {1, 1}));
    assert((kd.nearest(1, 0, ans, 0) && ans == pair < ll, int > {0, 4}));
    assert(kd.query(1, 0, 1, 0) == 2);
    assert(kd.query(2, 0, 1, 1) == 0);
    kd.build({{-1000000000LL, -1000000000LL}});
    assert(kd.nearest(1000000000LL, 1000000000LL, ans) && ans.first ==
           8000000000000000000LL);
    kd.build({}); assert(! kd.nearest(0, 0, ans));
    mt19937_64 rng(712312);
    auto coordinate = [&]() -> ll {
        if (rng() % 3) return (ll)(rng() % 31) - 15;
        return (ll)(rng() % 2000000001LL) - 1000000000LL;
    };
    for (int tc = 0; tc < 500; ++ tc) {
        int n = rng() % 101;
        V < Point > ps;
        for (int i = 0; i < n; ++ i)
            ps.push_back({coordinate(), coordinate()});
        if (n > 1 && tc % 3 == 0) ps.back() = ps[0];
        kd.build(ps);
        for (int q = 0; q < 200; ++ q) {
            ll x = coordinate(), y = coordinate();
            int exclude = n && rng() % 2 ? rng() % n : -1;
            if (n && q % 3 == 0) {
                auto p = ps[rng() % n]; x = p.first; y = p.second;
            }
            auto want = brute_nearest(ps, x, y, exclude);
            assert(kd.nearest(x, y, ans, exclude) == bool(want));
            if (want) assert(ans == *want);
            ll rx = coordinate(), ry = coordinate();
            if (q % 4) {
                if (x > rx) swap(x, rx);
                if (y > ry) swap(y, ry);
            }
            assert(kd.query(x, y, rx, ry) == brute_query(ps, x, y, rx, ry));
        }
        kd.build({}); assert(! kd.nearest(0, 0, ans));
    }
    V < Point > ps;
    for (int i = 0; i < 100000; ++ i) ps.push_back({i, i});
    kd.build(ps);
    assert((kd.nearest(50000, 50000, ans, 50000) && ans ==
            pair < ll, int > {2, 49999}));
    assert(kd.query(0, 0, 99999, 99999) == 100000);
    assert(kd.query(100, 100, 200, 200) == 101);
    ps.assign(10000, {0, 0}); kd.build(ps);
    assert((kd.nearest(0, 0, ans, 0) && ans == pair < ll, int > {0, 1}));
    assert(kd.query(0, 0, 0, 0) == 10000);
    cout << "kdtree_random OK: 500 sets, 100000 query pairs, large cases\n";
}
