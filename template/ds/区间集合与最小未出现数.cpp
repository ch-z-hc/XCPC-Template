#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// 整数区间集合，使用 ll 半开区间 [l, r)，允许负坐标；l >= r 视为空。
// s 中区间互不相交、互不相邻；insert 合并，erase 切开，clear 可复用。
// contains 判断点是否覆盖，mex(x) 返回 >= x 的最小未覆盖整数。
// insert / erase 为 O((k + 1) log m)，k 为触及区间数、m 为区间数。
// contains / mex 为 O(log m)，空间 O(m)；不使用模数。
// 不做端点 +/- 1，支持 LLONG_MIN / MAX 端点，但半开区间不能覆盖 LLONG_MAX。
struct IntervalSet {
    set < pair < ll, ll > > s;

    void clear() { s.clear(); }

    void insert(ll l, ll r) {
        if (l >= r) return;
        auto it = s.lower_bound({l, LLONG_MIN});
        if (it != s.begin() && prev(it)->second >= l) -- it;
        while (it != s.end() && it->first <= r) {
            l = min(l, it->first); r = max(r, it->second);
            it = s.erase(it);
        }
        s.insert({l, r});
    }

    void erase(ll l, ll r) {
        if (l >= r) return;
        auto it = s.lower_bound({l, LLONG_MIN});
        if (it != s.begin() && prev(it)->second > l) -- it;
        while (it != s.end() && it->first < r) {
            auto [a, b] = *it;
            it = s.erase(it);
            if (a < l) s.insert({a, l});
            if (b > r) { s.insert({r, b}); break; }
        }
    }

    bool contains(ll x) const {
        auto it = s.upper_bound({x, LLONG_MAX});
        return it != s.begin() && x < prev(it)->second;
    }

    ll mex(ll x = 0) const {
        auto it = s.upper_bound({x, LLONG_MAX});
        if (it != s.begin() && x < prev(it)->second) return prev(it)->second;
        return x;
    }
};

// IntervalSet seg; seg.insert(1, 4); seg.erase(2, 3);
// bool covered = seg.contains(1); ll x = seg.mex(1); // 2。
