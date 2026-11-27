/*
在 [1, h] 中枚举所有商同时不变的极大闭区间；nums 非负，up 指定向上取整。
调用 visit(l,r)，k 个分子、B 个区块耗时 O(kB)，额外空间 O(1)。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

template < class F >
void quotient_blocks(const V < ll > &nums, ll h, bool up, F visit) {
    for (ll l = 1; l <= h;) {
        ll r = h;
        for (ll n : nums) {
            ll x = n - (up && n > 0);
            if (x >= l) r = min(r, x / (x / l));
        }
        visit(l, r);
        if (r == h) break;
        l = r + 1;
    }
}

/*
quotient_blocks({30, 50}, 100, false, [&](ll l, ll r) {
    cout << l << ' ' << r << '\n';
});
*/
