#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// 64 位线性基；p 下标对应 bit 0..62，默认处理非负 ll。
// 单次插入、表示判断和最大异或查询均为 O(63)，空间 O(63)。

struct Basis {
    array < unsigned long long, 63 > p{};

    void clear() { p.fill(0); }
    bool insert(unsigned long long x) {
        for (int i = 62; i >= 0; -- i) if (x >> i & 1ULL) {
            if (p[i]) x ^= p[i];
            else { p[i] = x; return true; }
        }
        return false;
    }
    bool contains(unsigned long long x) const {
        for (int i = 62; i >= 0; -- i) if (x >> i & 1ULL) {
            if (! p[i]) return false;
            x ^= p[i];
        }
        return true;
    }
    unsigned long long max_xor(unsigned long long x = 0) const {
        for (int i = 62; i >= 0; -- i) x = max(x, x ^ p[i]);
        return x;
    }

    // 不同异或值从小到大第 k 个，k 为 0-based，包含空集的 0。
    // 无此项返回 nullopt；O(63^2)，不修改原线性基。
    optional < unsigned long long > kth(unsigned long long k) const {
        auto a = p;
        int n = 0;
        for (int i = 0; i < 63; ++ i) if (a[i]) {
            for (int j = 0; j < i; ++ j) if (a[i] >> j & 1ULL) a[i] ^= a[j];
            n ++;
        }
        if (k >= (1ULL << n)) return nullopt;
        unsigned long long ans = 0;
        for (int i = 0, j = 0; i < 63; ++ i)
            if (a[i]) { if (k >> j & 1ULL) ans ^= a[i]; j ++; }
        return ans;
    }
};

// 调用示例：
// Basis base;
// base.insert(x);
// auto ans = base.max_xor();
