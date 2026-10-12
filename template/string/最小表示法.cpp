#include <bits/stdc++.h>

using namespace std;

// Booth 最小表示法，返回字典序最小循环移位的起点，字符串下标 0-based。
// 多个相同最优旋转时返回最小起点；空串返回 0。时间 O(n)，额外空间 O(1)。

int min_rotation(const string &s) {
    int n = int(s.size()), i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        unsigned char a = s[(i + k) % n], b = s[(j + k) % n];
        if (a == b) { k ++; continue; }
        if (a > b) {
            i += k + 1;
            if (i <= j) i = j + 1;
        } else {
            j += k + 1;
            if (j <= i) j = i + 1;
        }
        k = 0;
    }
    return n ? min(i, j) : 0;
}

// 调用示例：int p = min_rotation(s); string t = s.substr(p) + s.substr(0, p);
