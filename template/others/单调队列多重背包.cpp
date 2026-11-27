/*
原地加入一种限量物品，w > 0，价值可负，每种 O(容量)。
恰好装满时除 dp[0] 外初始化为 neg；只限制总体积时全填 0。中间量须放入 ll。
*/

#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
typedef long long ll;
using namespace std;

void bounded_knapsack(V < ll > &dp, int w, ll v, int cnt, ll neg = -(1ll << 62)) {
    int n = int(dp.size()) - 1;
    for (int r = 0; r < w && r <= n; r ++) {
        deque < pair < int, ll > > q;
        for (int k = 0; k <= (n - r) / w; k ++) {
            int p = r + k * w;
            while (!q.empty() && q.front().fi < k - cnt) q.pop_front();
            if (dp[p] != neg) {
                ll val = dp[p] - k * v;
                while (!q.empty() && q.back().se <= val) q.pop_back();
                q.push_back({k, val});
            }
            dp[p] = q.empty() ? neg : q.front().se + k * v;
        }
    }
}

// bounded_knapsack(dp, w, v, cnt);
