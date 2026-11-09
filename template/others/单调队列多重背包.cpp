#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

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

// 用法：bounded_knapsack(dp, w, v, cnt);
