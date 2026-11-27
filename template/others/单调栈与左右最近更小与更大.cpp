// 求 a[1..n] 各点左右最近的严格更小或更大元素，O(n)。左、右无解分别记 0、n + 1。

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

pair < V < int >, V < int > > monotonic_bounds(const V < ll > &a,
    bool greater = false) {
    int n = int(a.size()) - 1;
    V < int > left(n + 1), right(n + 1, n + 1), st;
    for (int i = 1; i <= n; i ++) {
        while (!st.empty() &&
               (greater ? a[st.back()] <= a[i] : a[st.back()] >= a[i]))
            st.pop_back();
        if (!st.empty()) left[i] = st.back();
        st.push_back(i);
    }
    st.clear();
    for (int i = n; i >= 1; i --) {
        while (!st.empty() &&
               (greater ? a[st.back()] <= a[i] : a[st.back()] >= a[i]))
            st.pop_back();
        if (!st.empty()) right[i] = st.back();
        st.push_back(i);
    }
    return {left, right};
}

/*
V < ll > a{0, 3, 1, 2};
auto [l, r] = monotonic_bounds(a); auto greater = monotonic_bounds(a, true);
*/
