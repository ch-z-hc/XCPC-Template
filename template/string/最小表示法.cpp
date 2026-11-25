#include <bits/stdc++.h>

using namespace std;

// 返回字典序最小循环移位的起点，O(n)。下标从 0 数，并列取最小下标，空串返回 0。

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

// int p = min_rotation(s); string t = s.substr(p) + s.substr(0, p);
