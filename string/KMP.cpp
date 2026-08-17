#include <bits/stdc++.h>
using namespace std;

// KMP 字符串匹配；返回所有匹配起点，字符串下标 0-based，支持重叠匹配。
// 复杂度 O(text.size() + pattern.size())，空间 O(pattern.size())。
vector<int> prefix_function(const string &pattern) {
    vector<int> prefix(pattern.size());
    for (int i = 1; i < int(pattern.size()); ++i) {
        int matched = prefix[i - 1];
        while (matched > 0 && pattern[i] != pattern[matched]) matched = prefix[matched - 1];
        if (pattern[i] == pattern[matched]) ++matched;
        prefix[i] = matched;
    }
    return prefix;
}

vector<int> kmp_search(const string &text, const string &pattern) {
    if (pattern.empty()) {
        vector<int> positions(text.size() + 1);
        iota(positions.begin(), positions.end(), 0);
        return positions;
    }
    vector<int> prefix = prefix_function(pattern), positions;
    int matched = 0;
    for (int i = 0; i < int(text.size()); ++i) {
        while (matched > 0 && text[i] != pattern[matched]) matched = prefix[matched - 1];
        if (text[i] == pattern[matched]) ++matched;
        if (matched == int(pattern.size())) {
            positions.push_back(i - matched + 1);
            matched = prefix[matched - 1];
        }
    }
    return positions;
}

// Usage:
// vector<int> positions = kmp_search(text, pattern);
