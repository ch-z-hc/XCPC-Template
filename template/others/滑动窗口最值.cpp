#include <bits/stdc++.h>

#define V vector

using namespace std;

// 滑动窗口最值；数组下标 0-based，要求 1 <= k <= a.size()。
// 复杂度 O(n)，空间 O(k)。

template < class T >
V < T > window_min(const V < T > &a, int k) {
    deque < int > q;
    V < T > ans;
    for (int i = 0; i < int(a.size()); ++ i) {
        while (! q.empty() && q.front() <= i - k) q.pop_front();
        while (! q.empty() && a[q.back()] >= a[i]) q.pop_back();
        q.push_back(i);
        if (i + 1 >= k) ans.push_back(a[q.front()]);
    }
    return ans;
}

template < class T >
V < T > window_max(const V < T > &a, int k) {
    deque < int > q;
    V < T > ans;
    for (int i = 0; i < int(a.size()); ++ i) {
        while (! q.empty() && q.front() <= i - k) q.pop_front();
        while (! q.empty() && a[q.back()] <= a[i]) q.pop_back();
        q.push_back(i);
        if (i + 1 >= k) ans.push_back(a[q.front()]);
    }
    return ans;
}

// 调用示例：
// auto ans = window_min(a, k);
