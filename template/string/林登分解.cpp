/*
将字节串分解为按字典序不增的 Lyndon 串，O(n)。字符按无符号字节比较，相同因子分别保留。
第 i 段为 [pos[i], pos[i + 1])，空串返回 {0}。
*/

#include <bits/stdc++.h>

std::vector<int> duval(const std::string &s) {
    int n = int(s.size());
    std::vector<int> pos{0};
    for (int i = 0; i < n;) {
        int j = i + 1, k = i;
        while (j < n && (unsigned char)s[k] <= (unsigned char)s[j]) {
            if ((unsigned char)s[k] < (unsigned char)s[j])
                k = i;
            else
                ++ k;
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

// auto pos = duval("aabab");
