#include <bits/stdc++.h>

typedef unsigned long long ull;

using namespace std;

// 只处理低 63 位，要求 0 <= x < 2^63。insert(x) 返回 x 能否扩充线性基。
// contains(x) 判断 x 能否由已有数异或得到，max_xor(x) 求从 x 出发能得到的最大值。
// 这三种操作各 O(63)，空间 O(63)。clear() 清空线性基。

struct Basis {
    array < ull, 63 > p{};

    void clear() { p.fill(0); }
    bool insert(ull x) {
        for (int i = 62; i >= 0; -- i) if (x >> i & 1ULL) {
            if (p[i]) x ^= p[i];
            else { p[i] = x; return true; }
        }
        return false;
    }
    bool contains(ull x) {
        for (int i = 62; i >= 0; -- i) if (x >> i & 1ULL) {
            if (! p[i]) return false;
            x ^= p[i];
        }
        return true;
    }
    ull max_xor(ull x = 0) {
        for (int i = 62; i >= 0; -- i) x = max(x, x ^ p[i]);
        return x;
    }

    // 不同异或值从小到大第 k 个，k 为 0-based，包含空集的 0。
    // 无此项返回 false；成功时写入 ans；O(63^2)，不修改原线性基。
    bool kth(ull k, ull &ans) {
        auto a = p;
        int n = 0;
        for (int i = 0; i < 63; ++ i) if (a[i]) {
            for (int j = 0; j < i; ++ j) if (a[i] >> j & 1ULL) a[i] ^= a[j];
            n ++;
        }
        if (k >= (1ULL << n)) return false;
        ans = 0;
        for (int i = 0, j = 0; i < 63; ++ i)
            if (a[i]) {
                if (k >> j & 1ULL) ans ^= a[i];
                j ++;
            }
        return true;
    }
};

// Basis base;
// base.insert(x);
// auto ans = base.max_xor();
