/*
0-based 闭区间双模哈希，可跨对象比较。先比长度，再比哈希，相同哈希仍可能碰撞。
建表 O(n)，取哈希 O(1)，查两个后缀的 LCP 为 O(log n)。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct Hash {
    static const i64 mod1 = 1000000007, mod2 = 1000000009, base = 911382323;
    int n = 0;
    std::vector<i64> h1, h2, p1, p2;

    Hash(const std::string &s = "") { build(s); }

    void build(const std::string &s) {
        n = int(s.size());
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);
        p1.assign(n + 1, 1);
        p2.assign(n + 1, 1);
        for (int i = 0; i < n; i ++) {
            int x = (unsigned char)s[i] + 1;
            h1[i + 1] = (h1[i] * base + x) % mod1;
            h2[i + 1] = (h2[i] * base + x) % mod2;
            p1[i + 1] = p1[i] * base % mod1;
            p2[i + 1] = p2[i] * base % mod2;
        }
    }

    std::pair<i64, i64> query(int l, int r) {
        if (l > r) return {0, 0};
        i64 x = (h1[r + 1] - h1[l] * p1[r - l + 1]) % mod1;
        i64 y = (h2[r + 1] - h2[l] * p2[r - l + 1]) % mod2;
        return {(x + mod1) % mod1, (y + mod2) % mod2};
    }

    int lcp(int u, int v) {
        int l = 0, r = n - std::max(u, v);
        while (l < r) {
            int mid = (l + r + 1) >> 1;
            if (query(u, u + mid - 1) == query(v, v + mid - 1))
                l = mid;
            else
                r = mid - 1;
        }
        return l;
    }
};
const i64 Hash::mod1, Hash::mod2, Hash::base;

/*
Hash h; h.build("ababa");
bool same = h.query(0, 2) == h.query(2, 4); int len = h.lcp(0, 2);
*/
