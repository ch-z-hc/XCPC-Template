#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 每次加入一种物品，体积 w > 0，价值 v 可为负，最多选 cnt 件。原地更新 dp[0..容量]。
// 要求恰好装满时，初始 dp[0] = 0，其余填 neg。只限制总体积时，初始全填 0。
// neg 表示不可达，其余中间量须能放进 ll。每种物品耗时 O(容量)，额外空间 O(容量 / w + 1)。
void bounded_knapsack(V < ll > &dp, int w, ll v, int cnt,
                      ll neg = -(1ll << 62)) {
    int n = int(dp.size()) - 1;
    for (int r = 0; r < w && r <= n; ++ r) {
        deque < pair < int, ll > > q;
        for (int k = 0; k <= (n - r) / w; ++ k) {
            int p = r + k * w;
            while (! q.empty() && q.front().first < k - cnt) q.pop_front();
            if (dp[p] != neg) {
                ll val = dp[p] - k * v;
                while (! q.empty() && q.back().second <= val) q.pop_back();
                q.push_back({k, val});
            }
            dp[p] = q.empty() ? neg : q.front().second + k * v;
        }
    }
}

// bounded_knapsack(dp, w, v, cnt);
