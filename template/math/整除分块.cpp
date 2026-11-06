#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 对非负 ll 整数 n，返回 n / l 相同的所有最大区间 [l, r]；结果下标 0-based。
// n = 0 返回空集；时间与空间 O(sqrt(n))，须确保区间数组能放入内存。

V < pair < ll, ll > > div_blocks(ll n) {
    V < pair < ll, ll > > ans;
    for (ll l = 1; l <= n;) {
        ll r = n / (n / l);
        ans.push_back({l, r});
        if (r == n) break;
        l = r + 1;
    }
    return ans;
}

// 用法：for (auto [l, r] : div_blocks(n)) { /* n / l 在区间内相同。 */ }
