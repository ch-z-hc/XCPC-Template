/*
分治求一层分段 DP，二分栈求顺序递推，均取最小值。区间费用 O(1) 时两者均为 O(n log n)。
分治要求最左最优决策不减；二分栈要求较晚决策一旦严格占优，此后始终严格占优。
费用传 1-based 闭区间，有限转移须放入 i64；分层 DP 的不可达状态填 inf，有限值须小于 inf。
二分栈取 f[0] = base，f[i] = min_{0 <= j < i}(f[j] + cost(j + 1, i))，同值取较小决策。
要求 n >= 0、各状态可达，cost 不依赖未算出的状态。四边形不等式可保证严格占优条件，
仅有最优决策单调还不够。
文件尾用区间和的平方作费用，要求原数组非负。
*/

#include <bits/stdc++.h>

using i64 = long long;

template <class Cost>
std::vector<i64> dc_dp(const std::vector<i64> &prev, Cost cost, i64 inf = (1ll << 62)) {
    int n = int(prev.size()) - 1;
    std::vector<i64> cur(n + 1, inf);
    auto solve = [&](auto &&self, int l, int r, int lo, int hi) -> void {
        if (l > r) return;
        int mid = (l + r) >> 1, best = lo;
        for (int j = lo; j <= std::min(hi, mid - 1); j ++)
            if (prev[j] < inf) {
                i64 val = prev[j] + cost(j + 1, mid);
                if (val < cur[mid]) cur[mid] = val, best = j;
            }
        self(self, l, mid - 1, lo, best);
        self(self, mid + 1, r, best, hi);
    };
    solve(solve, 1, n, 0, n - 1);
    return cur;
}

template <class Cost> std::vector<i64> binary_stack_dp(int n, Cost cost, i64 base = 0) {
    std::vector<i64> f(n + 1);
    f[0] = base;
    std::deque<std::pair<int, int>> q;
    q.push_back({0, 1});
    auto val = [&](int j, int i) {
        return f[j] + cost(j + 1, i);
    };
    for (int i = 1; i <= n; i ++) {
        while (q.size() > 1 && q[1].second <= i) q.pop_front();
        f[i] = val(q.front().first, i);
        if (i == n) break;
        while (!q.empty()) {
            int l = std::max(i + 1, q.back().second);
            if (val(i, l) >= val(q.back().first, l)) break;
            q.pop_back();
        }
        if (q.empty()) {
            q.push_back({i, i + 1});
        } else {
            int j = q.back().first;
            if (val(i, n) >= val(j, n)) continue;
            int l = std::max(i + 1, q.back().second), r = n;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (val(i, mid) < val(j, mid))
                    r = mid;
                else
                    l = mid + 1;
            }
            q.push_back({i, l});
        }
    }
    return f;
}

/*
auto cost = [&](int l, int r) { i64 s = sum[r] - sum[l - 1]; return s * s; };
std::vector<i64> prev(n + 1, 1ll << 62); prev[0] = 0;
for (int t = 1; t <= k; t ++) prev = dc_dp(prev, cost);
auto f = binary_stack_dp(n, cost);
*/
