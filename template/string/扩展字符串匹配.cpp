/*
求一个串或模式串与每个后缀的最长公共前缀，O(|s| + |t|)。位置从 0 数，空模式的匹配长度全为 0。
非空串的 z[0] 为串长。exkmp(s, t) 以 s 为文本、t 为模式。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

V < int > get_z(const string &s) {
    int n = int(s.size());
    V < int > z(n);
    if (n) z[0] = n;
    for (int i = 1, l = 0, r = -1; i < n; ++ i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i] ++;
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    return z;
}

V < int > exkmp(const string &s, const string &t) {
    int n = int(s.size()), m = int(t.size());
    V < int > z = get_z(t), ans(n);
    for (int i = 0, l = 0, r = -1; i < n; ++ i) {
        if (i <= r) ans[i] = min(r - i + 1, z[i - l]);
        while (ans[i] < m && i + ans[i] < n && t[ans[i]] == s[i + ans[i]])
            ans[i] ++;
        if (i + ans[i] - 1 > r) l = i, r = i + ans[i] - 1;
    }
    return ans;
}

/*
auto z = get_z("ababa");
auto len = exkmp("ababa", "aba");
*/
