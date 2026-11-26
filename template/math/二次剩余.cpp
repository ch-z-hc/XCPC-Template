// 求 x² 与 a 模 p 同余的较小非负根，无解返回 -1。p 须为 [2, INT_MAX] 内的质数，期望 O(log p)。

#include <bits/stdc++.h>

using i64 = long long;

i64 sqrt_mod(i64 a, i64 p) {
    a %= p;
    if (a < 0) a += p;
    if (!a || p == 2) return a;
    auto qpow = [&](i64 x, i64 k) {
        i64 ans = 1;
        for (; k; k >>= 1, x = x * x % p)
            if (k & 1) ans = ans * x % p;
        return ans;
    };
    if (qpow(a, (p - 1) / 2) != 1) return -1;

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<i64> pick(0, p - 1);
    i64 b, w;
    do {
        b = pick(rng);
        w = (b * b % p - a + p) % p;
    } while (qpow(w, (p - 1) / 2) != p - 1);

    auto mul = [&](std::pair<i64, i64> u, std::pair<i64, i64> v) {
        return std::pair<i64, i64>{(u.first * v.first + u.second * v.second % p * w) % p,
                                   (u.first * v.second + u.second * v.first) % p};
    };
    std::pair<i64, i64> ans{1, 0}, x{b, 1};
    for (i64 k = (p + 1) / 2; k; k >>= 1, x = mul(x, x))
        if (k & 1) ans = mul(ans, x);
    return std::min(ans.first, p - ans.first);
}

// i64 x = sqrt_mod(10, 13);
