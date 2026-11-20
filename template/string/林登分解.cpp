#include <bits/stdc++.h>

#define V vector

using namespace std;

// 按字典序把字节串拆成若干不增的 Lyndon 串。返回分界点，第 i 段是 [pos[i], pos[i + 1])。
// 相同因子分开保留，空串返回 {0}。字符按无符号字节比较，时间和空间 O(n)。

V < int > duval(const string &s) {
    int n = int(s.size());
    V < int > pos{0};
    for (int i = 0; i < n; ) {
        int j = i + 1, k = i;
        while (j < n && (unsigned char)s[k] <= (unsigned char)s[j]) {
            if ((unsigned char)s[k] < (unsigned char)s[j]) k = i;
            else ++ k;
            ++ j;
        }
        int len = j - k;
        while (i <= k) {
            i += len;
            pos.push_back(i);
        }
    }
    return pos;
}

// auto pos = duval(s);
