// 求 a[1..n] 的最长严格递增或非递减子序列，O(n log n)。返回一组原数组下标。

#include <bits/stdc++.h>

template <class T> std::vector<int> lis(const std::vector<T> &a, bool strict = true) {
    std::vector<T> tail;
    std::vector<int> pos, pre(a.size(), 0);
    for (int i = 1; i < int(a.size()); i ++) {
        int k = int((strict ? std::lower_bound(tail.begin(), tail.end(), a[i])
                            : std::upper_bound(tail.begin(), tail.end(), a[i])) -
                    tail.begin());
        if (k) pre[i] = pos[k - 1];
        if (k == int(tail.size()))
            tail.push_back(a[i]), pos.push_back(i);
        else
            tail[k] = a[i], pos[k] = i;
    }
    std::vector<int> ans;
    for (int i = pos.empty() ? 0 : pos.back(); i; i = pre[i]) ans.push_back(i);
    std::reverse(ans.begin(), ans.end());
    return ans;
}

// auto ids = lis(a); auto nondecreasing = lis(a, false);
