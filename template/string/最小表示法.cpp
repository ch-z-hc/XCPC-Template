// 返回字典序最小循环移位的起点，O(n)。下标从 0 数，并列取最小下标，空串返回 0。

#include <bits/stdc++.h>

int min_rotation(const std::string &s) {
    int n = int(s.size()), i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        unsigned char a = s[(i + k) % n], b = s[(j + k) % n];
        if (a == b) {
            k ++;
        } else {
            int &loser = a > b ? i : j;
            loser += k + 1;
            if (i == j) j ++;
            k = 0;
        }
    }
    return std::min(i, j);
}

// int p = min_rotation(s); std::string t = s.substr(p) + s.substr(0, p);
