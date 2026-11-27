/*
先复制「素性测试与整数分解」。求 ull 范围内质数 p 的最小原根，p = 2 时取 1。
先分解 p - 1，设其有 t 个不同质因子，枚举至答案 g 的时间为 O(g t log² p)。
*/

#include <bits/stdc++.h>

#define V vector

typedef unsigned long long ull;

using namespace std;

V < ull > factor(ull n);
ull rho_pow(ull a, ull b, ull mod);

ull primitive_root(ull p) {
    if (p == 2) return 1;
    auto q = factor(p - 1);
    q.erase(unique(q.begin(), q.end()), q.end());
    for (ull g = 2;; g ++) {
        bool ok = true;
        for (ull x : q)
            if (rho_pow(g, (p - 1) / x, p) == 1) {
                ok = false;
                break;
            }
        if (ok) return g;
    }
}

// ull g = primitive_root(p);
