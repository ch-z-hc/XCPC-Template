/*
求模式串前缀函数及其在文本中的全部匹配位置，O(|s| + |t|)。匹配可重叠，下标从 0 数。
空模式匹配 0..|s| 的全部位置。
*/

#include <bits/stdc++.h>

std::vector<int> get_pi(const std::string &t) {
    std::vector<int> pi(t.size());
    for (int i = 1, j = 0; i < int(t.size()); i ++) {
        while (j > 0 && t[i] != t[j]) j = pi[j - 1];
        if (t[i] == t[j]) ++ j;
        pi[i] = j;
    }
    return pi;
}

std::vector<int> kmp(const std::string &s, const std::string &t) {
    if (t.empty()) {
        std::vector<int> ans(s.size() + 1);
        std::iota(ans.begin(), ans.end(), 0);
        return ans;
    }
    std::vector<int> pi = get_pi(t), ans;
    int j = 0;
    for (int i = 0; i < int(s.size()); i ++) {
        while (j > 0 && s[i] != t[j]) j = pi[j - 1];
        if (s[i] == t[j]) ++ j;
        if (j == int(t.size())) {
            ans.push_back(i - j + 1);
            j = pi[j - 1];
        }
    }
    return ans;
}

// auto pi = get_pi("aba"); auto pos = kmp("ababa", "aba");
