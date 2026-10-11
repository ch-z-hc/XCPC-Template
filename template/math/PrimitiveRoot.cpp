#include <bits/stdc++.h>

#define V vector

typedef unsigned long long ull;

using namespace std;

// 依赖 PollardRho.cpp：先复制其中的 factor、rho_pow 及其依赖。
// p 必须是质数，支持整个 ull 范围；返回最小原根，p = 2 时返回 1。
// 分解 p - 1 后枚举候选 g，代价为分解加 O(g * omega(p - 1) * log p)。
// omega 为不同质因子数；额外空间 O(log p)，无跨调用状态。
V < ull > factor(ull n);
ull rho_pow(ull a, ull b, ull mod);

ull primitive_root(ull p) {
    if (p == 2) return 1;
    auto q = factor(p - 1);
    q.erase(unique(q.begin(), q.end()), q.end());
    for (ull g = 2; ; ++ g) {
        bool ok = true;
        for (ull x : q) if (rho_pow(g, (p - 1) / x, p) == 1) {
            ok = false; break;
        }
        if (ok) return g;
    }
}

// 调用：ull g = primitive_root(p);
