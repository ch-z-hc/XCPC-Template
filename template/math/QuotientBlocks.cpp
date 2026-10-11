#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 对正整数 n，返回 n / l 取值相同的所有最大区间 [l, r]；结果下标 0-based。
// 复杂度 O(sqrt(n))，区间数量 O(sqrt(n))。

V < pair < ll, ll > > div_blocks(ll n) {
    V < pair < ll, ll > > ans;
    for (ll l = 1; l <= n;) {
        ll r = n / (n / l);
        ans.push_back({l, r});
        l = r + 1;
    }
    return ans;
}

// 调用示例：
// for (auto [l, r] : div_blocks(n)) { /* n / l 在区间内相同。 */ }
