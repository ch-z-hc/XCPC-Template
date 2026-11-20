#include <bits/stdc++.h>

#define V vector

using namespace std;

// lis(a) 在 a[1..n] 中找一条最长严格递增子序列，传 false 改为非递减。
// 返回一组原数组下标，结果的大小就是最优长度。时间 O(n log n)，空间 O(n)。
template < class T >
V < int > lis(const V < T > &a, bool strict = true) {
    V < T > tail;
    V < int > pos, pre(a.size(), 0);
    for (int i = 1; i < int(a.size()); ++ i) {
        int k = int((strict ? lower_bound(tail.begin(), tail.end(), a[i]) :
                     upper_bound(tail.begin(), tail.end(), a[i])) - tail.begin());
        if (k) pre[i] = pos[k - 1];
        if (k == int(tail.size())) tail.push_back(a[i]), pos.push_back(i);
        else tail[k] = a[i], pos[k] = i;
    }
    V < int > ans;
    for (int i = pos.empty() ? 0 : pos.back(); i; i = pre[i]) ans.push_back(i);
    reverse(ans.begin(), ans.end());
    return ans;
}

// auto ids = lis(a); auto nondecreasing = lis(a, false);
