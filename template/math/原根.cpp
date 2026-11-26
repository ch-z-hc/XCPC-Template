/*
先复制「素性测试与整数分解」。求 u64 范围内质数 p 的最小原根，p = 2 时取 1。
先分解 p - 1，设其有 t 个不同质因子，枚举至答案 g 的时间为 O(g t log² p)。
*/

#include <bits/stdc++.h>

using u64 = unsigned long long;

std::vector<u64> factor(u64 n);
u64 rho_pow(u64 a, u64 b, u64 mod);

u64 primitive_root(u64 p) {
    if (p == 2) return 1;
    auto q = factor(p - 1);
    q.erase(std::unique(q.begin(), q.end()), q.end());
    for (u64 g = 2;; g ++) {
        bool ok = true;
        for (u64 x : q)
            if (rho_pow(g, (p - 1) / x, p) == 1) {
                ok = false;
                break;
            }
        if (ok) return g;
    }
}

// u64 g = primitive_root(p);
