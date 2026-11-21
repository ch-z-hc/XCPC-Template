#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
从 1 到 n，列出所有 n / i 取值相同的极大闭区间 [l, r]。
n 为非负 ll，n = 0 返回空数组。时间和空间 O(sqrt(n))，全部区间都存入结果。
*/

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

// auto blocks = div_blocks(5); // [1, 1]、[2, 2]、[3, 5]
