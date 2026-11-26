/*
原地加入一种限量物品，w > 0，价值可负，每种 O(容量)。
恰好装满时除 dp[0] 外初始化为 neg；只限制总体积时全填 0。中间量须放入 i64。
*/

#include <bits/stdc++.h>

using i64 = long long;

void bounded_knapsack(std::vector<i64> &dp, int w, i64 v, int cnt, i64 neg = -(1ll << 62)) {
    int n = int(dp.size()) - 1;
    for (int r = 0; r < w && r <= n; r ++) {
        std::deque<std::pair<int, i64>> q;
        for (int k = 0; k <= (n - r) / w; k ++) {
            int p = r + k * w;
            while (!q.empty() && q.front().first < k - cnt) q.pop_front();
            if (dp[p] != neg) {
                i64 val = dp[p] - k * v;
                while (!q.empty() && q.back().second <= val) q.pop_back();
                q.push_back({k, val});
            }
            dp[p] = q.empty() ? neg : q.front().second + k * v;
        }
    }
}

// bounded_knapsack(dp, w, v, cnt);
