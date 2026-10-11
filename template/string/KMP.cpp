#include <bits/stdc++.h>

#define V vector

using namespace std;

// KMP 字符串匹配；返回所有匹配起点，字符串下标 0-based，支持重叠匹配。
// 复杂度 O(s.size() + t.size())；额外空间 O(t.size())，另需存放匹配结果。

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

// 调用示例：
// V < int > ans = kmp(s, t);
