#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
对每个长度为 k 的窗口取最小值或最大值。下标从 0 数，ans[i] 对应 a[i..i + k - 1]。
要求 1 <= k <= a.size()。耗时 O(n)，除结果外占 O(k) 空间。
*/

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

// auto ans = window_min(a, k);
