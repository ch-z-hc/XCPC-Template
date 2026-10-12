#include <bits/stdc++.h>
#include <cassert>

#include "../../../template/math/杜教筛.cpp"
#include "../../../template/math/扩展卢卡斯定理.cpp"
#include "../../../template/geometry/矩形面积并与周长并.cpp"

using namespace std;

// 洛谷题目输入适配；算法直接调用模板，不含独立实现。
int main(int argc, char **argv) {
    assert(argc == 2);
    ios::sync_with_stdio(0); cin.tie(0);
    string mode = argv[1];
    if (mode == "P4213") {
        int t; cin >> t;
        V < ll > a(t);
        ll n = 1;
        for (ll &x : a) { cin >> x; n = max(n, x); }
        DuJiao d;
        d.init(max(1, int(powl(n, 2.0L / 3))));
        // 本题 n < 2^31，phi 前缀和可以放入 ll。
        for (ll x : a) cout << ll(d.phi(x)) << ' ' << d.mu(x) << '\n';
    } else if (mode == "P4720") {
        ll n, k; int mod; cin >> n >> k >> mod;
        ExLucas c; c.init(mod);
        cout << c.choose(n, k) << '\n';
    } else if (mode == "P5490" || mode == "P1856") {
        int n; cin >> n;
        V < array < ll, 4 > > rects(n);
        for (auto &r : rects) cin >> r[0] >> r[1] >> r[2] >> r[3];
        RectUnion scan;
        auto [area, perimeter] = scan.solve(rects);
        cout << (mode == "P5490" ? area : perimeter) << '\n';
    } else {
        assert(false);
    }
}
