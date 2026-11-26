/*
求两个字节串的 LCS 长度。令 B = max(1, ceil(|t| / 64))，时间 O((|s| + 256)B)，空间 O(256B)。
较短串放第二个参数可缩短位集。
*/

#include <bits/stdc++.h>

using u64 = unsigned long long;

int bitset_lcs(const std::string &s, const std::string &t) {
    int n = int(t.size()), m = (n + 63) / 64;
    std::vector<std::vector<u64>> mask(256, std::vector<u64>(m));
    for (int i = 0; i < n; i ++) mask[(unsigned char)t[i]][i >> 6] |= 1ULL << (i & 63);
    std::vector<u64> d(m);
    for (unsigned char c : s) {
        u64 carry = 1, borrow = 0;
        for (int i = 0; i < m; i ++) {
            u64 old = d[i], x = old | mask[c][i], y = (old << 1) | carry;
            carry = old >> 63;
            u64 z = x - y - borrow;
            u64 next = x < y || (borrow && x == y);
            d[i] = x & ~z;
            borrow = next;
        }
    }
    int ans = 0;
    for (auto x : d) ans += __builtin_popcountll(x);
    return ans;
}

// int ans = bitset_lcs(s, t);
