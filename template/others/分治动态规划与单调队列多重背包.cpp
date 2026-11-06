#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 分治优化一层 DP：cur[i] = min_{0 <= j < i}(prev[j] + cost(j + 1, i))。
// prev[0..n]，区间为 1-based 闭区间；要求最左最优决策单调不减。
// 不可达值为 inf；所有有限转移不溢出 ll 且 < inf，不满足单调性不能使用。
// cost 为 O(1) 时一层 O(n log n)，空间 O(n)。
template < class Cost >
V < ll > dc_dp(const V < ll > &prev, Cost cost, ll inf = (1ll << 62)) {
    int n = int(prev.size()) - 1;
    V < ll > cur(n + 1, inf);
    auto solve = [&](auto &&self, int l, int r, int lo, int hi) -> void {
        if (l > r) return;
        int mid = (l + r) >> 1, best = lo;
        for (int j = lo; j <= min(hi, mid - 1); ++ j) if (prev[j] < inf) {
            ll val = prev[j] + cost(j + 1, mid);
            if (val < cur[mid]) cur[mid] = val, best = j;
        }
        self(self, l, mid - 1, lo, best);
        self(self, mid + 1, r, best, hi);
    };
    solve(solve, 1, n, 0, n - 1);
    return cur;
}

// 多重背包加入一种物品：体积 w > 0，价值 v，数量 cnt >= 0。
// dp[c] 为容量 c 的最优价值，精确体积用 dp[0]=0、其他填 neg。
// 不超过容量可全填 0；neg 为不可达哨兵，有限中间运算不得溢出 ll。
// 单种物品 O(容量) 时间、O(容量 / w + 1) 空间，允许负价值。
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

// 用法：auto cur = dc_dp(prev, cost); bounded_knapsack(dp, w, v, cnt);
