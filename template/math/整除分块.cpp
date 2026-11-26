/*
枚举 n / i 相同的极大闭区间，1 <= i <= n。非负 i64 输入，n = 0 时为空。
时间 O(sqrt(n))，返回数组会保存全部区间。
*/

#include <bits/stdc++.h>

using i64 = long long;

std::vector<std::pair<i64, i64>> div_blocks(i64 n) {
    std::vector<std::pair<i64, i64>> ans;
    for (i64 l = 1; l <= n;) {
        i64 r = n / (n / l);
        ans.push_back({l, r});
        if (r == n) break;
        l = r + 1;
    }
    return ans;
}

// auto blocks = div_blocks(5);
