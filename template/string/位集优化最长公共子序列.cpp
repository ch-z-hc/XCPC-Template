#include <bits/stdc++.h>

#define V vector

typedef unsigned long long ull;

using namespace std;

// 求两个字节串的 LCS 长度。第二个串按 64 位分块，可把较短的串放在第二个参数。
// 每块 64 位，设 B = max(1, ceil(|t| / 64))。时间 O((|s| + 256)B)，空间 O(256B)。
int bitset_lcs(const string &s, const string &t) {
    int n = int(t.size()), m = (n + 63) / 64;
    V < V < ull > > mask(256, V < ull > (m));
    for (int i = 0; i < n; ++ i) mask[(unsigned char)t[i]][i >> 6] |= 1ULL << (i & 63);
    V < ull > d(m);
    for (unsigned char c : s) {
        ull carry = 1, borrow = 0;
        for (int i = 0; i < m; ++ i) {
            ull old = d[i], x = old | mask[c][i], y = (old << 1) | carry;
            carry = old >> 63;
            ull z = x - y - borrow;
            ull next = x < y || (borrow && x == y);
            d[i] = x & ~z; borrow = next;
        }
    }
    int ans = 0;
    for (auto x : d) ans += __builtin_popcountll(x);
    return ans;
}

// int ans = bitset_lcs(s, t);
