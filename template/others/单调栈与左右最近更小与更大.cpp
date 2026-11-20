#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 对 a[1..n] 找每个位置左右最近的严格更小元素，传 true 改找严格更大元素。
// 返回下标，左边找不到记 0，右边找不到记 n + 1。相等的值跳过。时间和空间 O(n)。
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

// auto [left, right] = monotonic_bounds(a);
