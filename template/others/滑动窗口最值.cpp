#include <bits/stdc++.h>

template <class T> std::vector<T> window_min(const std::vector<T> &a, int k) {
    std::deque<int> q;
    std::vector<T> ans;
    for (int i = 0; i < int(a.size()); i ++) {
        while (!q.empty() && q.front() <= i - k) q.pop_front();
        while (!q.empty() && a[q.back()] >= a[i]) q.pop_back();
        q.push_back(i);
        if (i + 1 >= k) ans.push_back(a[q.front()]);
    }
    return ans;
}

template <class T> std::vector<T> window_max(const std::vector<T> &a, int k) {
    std::deque<int> q;
    std::vector<T> ans;
    for (int i = 0; i < int(a.size()); i ++) {
        while (!q.empty() && q.front() <= i - k) q.pop_front();
        while (!q.empty() && a[q.back()] <= a[i]) q.pop_back();
        q.push_back(i);
        if (i + 1 >= k) ans.push_back(a[q.front()]);
    }
    return ans;
}

// auto low = window_min(a, k), high = window_max(a, k);
