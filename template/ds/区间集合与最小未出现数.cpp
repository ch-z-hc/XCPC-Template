/*
维护整数半开区间的并，查询覆盖情况和从 x 起的第一个缺失值。LLONG_MAX 始终未覆盖。
现有 m 段，一次修改涉及 k 段，修改 O((k + 1) log m)，查询 O(log m)。
*/

#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

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

    bool contains(ll x) {
        auto it = s.upper_bound({x, LLONG_MAX});
        return it != s.begin() && x < prev(it)->second;
    }

    ll mex(ll x = 0) {
        auto it = s.upper_bound({x, LLONG_MAX});
        if (it != s.begin() && x < prev(it)->second) return prev(it)->second;
        return x;
    }
};

/*
IntervalSet seg; seg.insert(1, 4); seg.erase(2, 3);
bool covered = seg.contains(1); ll x = seg.mex(1);
seg.clear();
*/
