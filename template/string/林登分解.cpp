#include <bits/stdc++.h>

#define V vector

using namespace std;

// Duval 算法：将字符串分解为字典序不增的 Lyndon 串，O(n) 时间、O(n) 空间。
// 字符按 unsigned char 比较；返回 0-based 分界点，块为 [pos[i], pos[i+1])。
// 结果含首尾 0 和 n；空串返回 {0}，相等的因子分别保留。

V < int > lyndon_factorization(const string &s) {
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
        while (i <= k) { i += len; pos.push_back(i); }
    }
    return pos;
}

// auto pos = lyndon_factorization(s);
