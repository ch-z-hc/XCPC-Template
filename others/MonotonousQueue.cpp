#include <bits/stdc++.h>
using namespace std;

// 滑动窗口最值；数组下标 0-based，要求 1 <= window <= a.size()。
// 复杂度 O(n)，空间 O(window)。
template <class T>
vector<T> sliding_window_minimum(const vector<T> &a, int window) {
    deque<int> q; vector<T> result;
    for (int i = 0; i < int(a.size()); ++i) {
        while (!q.empty() && q.front() <= i - window) q.pop_front();
        while (!q.empty() && a[q.back()] >= a[i]) q.pop_back();
        q.push_back(i); if (i + 1 >= window) result.push_back(a[q.front()]);
    }
    return result;
}

template <class T>
vector<T> sliding_window_maximum(const vector<T> &a, int window) {
    deque<int> q; vector<T> result;
    for (int i = 0; i < int(a.size()); ++i) {
        while (!q.empty() && q.front() <= i - window) q.pop_front();
        while (!q.empty() && a[q.back()] <= a[i]) q.pop_back();
        q.push_back(i); if (i + 1 >= window) result.push_back(a[q.front()]);
    }
    return result;
}
