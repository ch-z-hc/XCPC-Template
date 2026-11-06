#include <bits/stdc++.h>

#define V vector

using namespace std;

// 极大周期子串 runs；先复制字符串章的“后缀数组”，输入任意字节字符串。
// 返回 {period, l, r}，0-based 半开区间 [l, r)，period 是最小周期。
// 每个区间长度 >= 2*period，且不能向任一侧扩展后仍保持该周期。
// 两种字典序的 Lyndon 根 + LCP / LCS；时间、空间 O(n log n)，结果排序去重。
// 参考 Bannai 等人的 runs 定理；不使用字符串哈希，无碰撞风险。

V < array < int, 3 > > enumerate_runs(const string &s) {
    int n = int(s.size());
    if (n < 2) return {};
    SA forward(s), backward(string(s.rbegin(), s.rend()));
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
                int p = j - i, right = forward.lcp(i, j);
                int left = i ? backward.lcp(n - i, n - j) : 0;
                if (left + right >= p) ans.push_back({p, i - left, j + right});
            }
            st.push_back(i);
        }
    }
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return ans;
}

// 用法：auto runs = enumerate_runs(s); // 依次读取 {period, l, r}。
