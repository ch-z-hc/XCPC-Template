/*
求一个串或模式串与每个后缀的最长公共前缀，O(|s| + |t|)。位置从 0 数，空模式的匹配长度全为 0。
非空串的 z[0] 为串长。exkmp(s, t) 以 s 为文本、t 为模式。
*/

#include <bits/stdc++.h>

std::vector<int> get_z(const std::string &s) {
    int n = int(s.size());
    std::vector<int> z(n);
    if (n) z[0] = n;
    for (int i = 1, l = 0, r = -1; i < n; i ++) {
        int &k = z[i];
        if (i <= r) k = std::min(r - i + 1, z[i - l]);
        while (i + k < n && s[k] == s[i + k]) k ++;
        if (i + k - 1 > r) l = i, r = i + k - 1;
    }
    return z;
}

std::vector<int> exkmp(const std::string &s, const std::string &t) {
    int n = int(s.size()), m = int(t.size());
    std::vector<int> z = get_z(t), ans(n);
    for (int i = 0, l = 0, r = -1; i < n; i ++) {
        int &k = ans[i];
        if (i <= r) k = std::min(r - i + 1, z[i - l]);
        while (k < m && i + k < n && t[k] == s[i + k]) k ++;
        if (i + k - 1 > r) l = i, r = i + k - 1;
    }
    return ans;
}

/*
auto z = get_z("ababa");
auto len = exkmp("ababa", "aba");
*/
