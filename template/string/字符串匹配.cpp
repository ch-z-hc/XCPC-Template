#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
kmp(s, t) 在文本 s 中找模式串 t，按从左到右的顺序返回起点，下标从 0 数。
匹配可重叠。t 为空时返回 0 到 s.size() 的所有位置。
时间 O(|s| + |t|)，除答案外占 O(|t|) 空间。
*/

V < int > get_pi(const string &t) {
    V < int > pi(t.size());
    for (int i = 1; i < int(t.size()); ++ i) {
        int j = pi[i - 1];
        while (j > 0 && t[i] != t[j]) j = pi[j - 1];
        if (t[i] == t[j]) ++ j;
        pi[i] = j;
    }
    return pi;
}

V < int > kmp(const string &s, const string &t) {
    if (t.empty()) {
        V < int > ans(s.size() + 1);
        iota(ans.begin(), ans.end(), 0);
        return ans;
    }
    V < int > pi = get_pi(t), ans;
    int j = 0;
    for (int i = 0; i < int(s.size()); ++ i) {
        while (j > 0 && s[i] != t[j]) j = pi[j - 1];
        if (s[i] == t[j]) ++ j;
        if (j == int(t.size())) {
            ans.push_back(i - j + 1);
            j = pi[j - 1];
        }
    }
    return ans;
}

// auto pos = kmp("ababa", "aba"); // {0, 2}
