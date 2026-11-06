#include <bits/stdc++.h>

#define V vector

using namespace std;

// Z 函数与扩展 KMP，字符串下标 0-based，时间和空间 O(n + m)。
// get_z(s)[i] 为 s 与 s[i..] 的 LCP，z[0] = n。
// exkmp(s, t)[i] 为 t 与 s[i..] 的 LCP；空模式串的结果全为 0。

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

// 用法：auto z = get_z(s); auto match = exkmp(s, t);
