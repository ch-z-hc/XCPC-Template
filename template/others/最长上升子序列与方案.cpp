#include <bits/stdc++.h>

#define V vector

using namespace std;

// 最长上升子序列及方案恢复；a 为 1-based，a[0] 不使用。
// strict 为真时严格递增，否则非递减；返回一组原下标，长度即最优长度。
// O(n log n) 时间、O(n) 空间；重复值按 strict 处理，不使用模数。
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
