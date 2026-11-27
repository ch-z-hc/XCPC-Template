/*
枚举 n / i 相同的极大闭区间，1 <= i <= n。非负 ll 输入，n = 0 时为空。
时间 O(sqrt(n))，返回数组会保存全部区间。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

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

// auto blocks = div_blocks(5);
