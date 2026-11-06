#include <bits/stdc++.h>

#define V vector

using namespace std;

// bitset LCS 长度；字符串下标 0-based，支持任意 byte（按 unsigned char 索引）。
// O(|s| ceil(|t| / 64) + 256 ceil(|t| / 64)) 时间，后者为预处理。
// 空间 O(256 ceil(|t| / 64))；只返回长度，不恢复具体公共子序列。
int bitset_lcs(const string &s, const string &t) {
    using ull = unsigned long long;
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

// 用法：int ans = bitset_lcs(s, t);
