/*
求满足 a^x 与 b 模 mod 同余的最小非负 x，1 <= mod <= INT_MAX，约定 a^0 = 1，无解返回 -1。
哈希表的期望时间与空间均为 O(sqrt(mod))。
bsgs 要求底数与模数互质，exbsgs 允许不互质。
*/

#include <bits/stdc++.h>

using i64 = long long;

i64 bsgs_inv(i64 a, i64 mod) {
    auto dfs = [&](auto &&self, i64 a_, i64 b, i64 &x, i64 &y) -> void {
        if (!b) {
            x = 1;
            y = 0;
            return;
        }
        i64 u, v;
        self(self, b, a_ % b, u, v);
        x = v;
        y = u - a_ / b * v;
    };
    i64 x, y;
    dfs(dfs, a, mod, x, y);
    return (x % mod + mod) % mod;
}

i64 bsgs(i64 a, i64 b, i64 mod) {
    if (mod == 1) return 0;
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    i64 m = i64(sqrtl(mod)) + 1, cur = 1;
    std::unordered_map<i64, i64> pos;
    pos.reserve(std::size_t(m * 2));
    for (i64 j = 0; j < m; j ++) {
        pos.emplace(cur, j);
        cur = cur * a % mod;
    }
    i64 step = bsgs_inv(cur, mod);
    cur = b;
    for (i64 i = 0; i <= m; i ++) {
        auto it = pos.find(cur);
        if (it != pos.end()) return i * m + it->second;
        cur = cur * step % mod;
    }
    return -1;
}

i64 exbsgs(i64 a, i64 b, i64 mod) {
    if (mod == 1) return 0;
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    i64 k = 1, cnt = 0;
    for (i64 g; (g = std::gcd(a, mod)) > 1;) {
        if (b == k) return cnt;
        if (b % g) return -1;
        b /= g;
        mod /= g;
        cnt ++;
        k = k * (a / g) % mod;
    }
    i64 ans = bsgs(a, b * bsgs_inv(k, mod) % mod, mod);
    return ans == -1 ? -1 : ans + cnt;
}

/*
i64 x = bsgs(2, 3, 5);
i64 y = exbsgs(2, 4, 8);
*/
