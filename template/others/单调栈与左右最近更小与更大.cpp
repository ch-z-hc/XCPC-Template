#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 1-based 数组 a，a[0] 不使用；返回左右最近的严格更小元素下标。
// greater = true 时改为严格更大；不存在时 left = 0，right = n + 1。
// 时间 O(n)，空间 O(n)；相等元素不算更小 / 更大。
pair < V < int >, V < int > > monotonic_bounds(const V < ll > &a,
                                              bool greater = false) {
    int n = int(a.size()) - 1;
    V < int > left(n + 1), right(n + 1, n + 1), st;
    for (int i = 1; i <= n; ++ i) {
        while (! st.empty() && (greater ? a[st.back()] <= a[i] : a[st.back()] >= a[i]))
            st.pop_back();
        if (! st.empty()) left[i] = st.back();
        st.push_back(i);
    }
    st.clear();
    for (int i = n; i >= 1; -- i) {
        while (! st.empty() && (greater ? a[st.back()] <= a[i] : a[st.back()] >= a[i]))
            st.pop_back();
        if (! st.empty()) right[i] = st.back();
        st.push_back(i);
    }
    return {left, right};
}

// 用法：auto [left, right] = monotonic_bounds(a);
