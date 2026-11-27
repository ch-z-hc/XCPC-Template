/*
先复制「后缀数组」。枚举至少含两个周期且不能左右延长的极大周期子串，时间 O(n log n)。
输入可含任意字节。基于 Bannai 等人的 runs 定理。
结果每项为 {最小周期, l, r}，使用 0-based 半开区间，按三元组排序并去重。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

V < array < int, 3 > > runs(const string &s) {
    int n = int(s.size());
    if (n < 2) return {};
    SA sa(s), rsa(string(s.rbegin(), s.rend()));
    V < array < int, 3 > > ans;
    for (int order = 0; order < 2; order ++) {
        string t = s;
        if (order)
            for (char &c : t) c = char(255 - (unsigned char)c);
        SA sorted(t);
        V < int > st{n};
        for (int i = n - 1; i >= 0; i --) {
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

// auto res = runs("ababab");
