#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 分治优化一层 DP：cur[i] = min_{0 <= j < i}(prev[j] + cost(j + 1, i))。
// prev[0..n]，区间为 1-based 闭区间；要求最左最优决策单调不减。
// prev 必须已经全部算出；不能直接用于依赖当前层 f[j] 的顺序递推。
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

// 二分栈：f[0] = base，f[i] = min_{0 <= j < i}(f[j] + cost(j + 1, i))。
// n >= 0，cost(l, r) 为 1-based 闭区间费用，所有状态可达。
// 对任意 a < b，若 b 在某个 i > b 严格优于 a，则在所有更大 i 上仍严格更优。
// 四边形不等式可保证此条件；仅有最终最优决策单调还不够。
// 同值保留较小决策；cost 为 O(1) 时 O(n log n) 时间、O(n) 空间。
// 所有费用与转移之和须在 ll 范围内；cost 不得依赖尚未算出的状态。
template < class Cost >
V < ll > binary_stack_dp(int n, Cost cost, ll base = 0) {
    V < ll > f(n + 1);
    f[0] = base;
    // 栈中存 {决策, 成为最优决策的起点}；队首移除已过期的区间。
    deque < pair < int, int > > q;
    q.push_back({0, 1});
    auto val = [&](int j, int i) { return f[j] + cost(j + 1, i); };
    for (int i = 1; i <= n; ++ i) {
        while (q.size() > 1 && q[1].second <= i) q.pop_front();
        f[i] = val(q.front().first, i);
        if (i == n) break;
        while (! q.empty()) {
            int l = max(i + 1, q.back().second);
            if (val(i, l) >= val(q.back().first, l)) break;
            q.pop_back();
        }
        if (q.empty()) {
            q.push_back({i, i + 1});
        } else {
            int j = q.back().first;
            if (val(i, n) >= val(j, n)) continue;
            int l = max(i + 1, q.back().second), r = n;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (val(i, mid) < val(j, mid)) r = mid;
                else l = mid + 1;
            }
            q.push_back({i, l});
        }
    }
    return f;
}

// 用法：cost(l, r) = (sum[r] - sum[l - 1])^2，要求原数组非负。
// auto cost = [&](int l, int r) { ll s = sum[r] - sum[l - 1]; return s * s; };
// V < ll > prev(n + 1, 1ll << 62); prev[0] = 0;
// for (int t = 1; t <= k; ++ t) prev = dc_dp(prev, cost); // 恰好 k 段
// auto f = binary_stack_dp(n, cost); // 段数不限，也可在 cost 中加入每段费用
