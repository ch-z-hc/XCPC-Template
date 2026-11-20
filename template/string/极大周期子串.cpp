#include <bits/stdc++.h>

#define V vector

using namespace std;

// 先复制「后缀数组」。runs(s) 返回 {最小周期, l, r}，区间为 [l, r)，下标从 0 数。
// 每段至少含两个周期，向左或向右再延长一个字符都会破坏该周期。
// 输入可含任意字节，结果排序去重。基于 Bannai 等人的 runs 定理，时间和空间 O(n log n)。

V < array < int, 3 > > runs(const string &s) {
    int n = int(s.size());
    if (n < 2) return {};
    SA sa(s), rsa(string(s.rbegin(), s.rend()));
    V < array < int, 3 > > ans;
    for (int order = 0; order < 2; ++ order) {
        // 反向字符序保留同一前缀规则；后缀字典序不能直接取 rk 的相反数。
        string t = s;
        if (order) for (char &c : t) c = char(255 - (unsigned char)c);
        SA sorted(t);
        V < int > st{n};
        for (int i = n - 1; i >= 0; -- i) {
            while (st.back() != n && sorted.rk[st.back()] > sorted.rk[i])
                st.pop_back();
            int j = st.back();
            if (j != n) {
                int p = j - i, right = sa.lcp(i, j);
                int left = i ? rsa.lcp(n - i, n - j) : 0;
                if (left + right >= p) ans.push_back({p, i - left, j + right});
            }
            st.push_back(i);
        }
    }
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return ans;
}

// auto res = runs(s); // 依次读取 {period, l, r}。
