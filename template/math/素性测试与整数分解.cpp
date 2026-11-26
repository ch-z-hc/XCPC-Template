/*
支持整个 u64 范围。确定性判素用 O(log n) 次模乘；Pollard–Rho 找因子期望约 O(sqrt(p)) 次模运算，
p 为最小质因子。一次倍增模乘为 O(log n)。
factor 对 n < 2 返回空表，其余按非降序返回质因子，保留重数。直接调用 rho 时须传入合数。
*/

#include <bits/stdc++.h>

using u64 = unsigned long long;

u64 rho_mul(u64 a, u64 b, u64 mod) {
    a %= mod;
    u64 ans = 0;
    for (; b; b >>= 1) {
        if (b & 1) ans = ans >= mod - a ? ans - (mod - a) : ans + a;
        a = a >= mod - a ? a - (mod - a) : a + a;
    }
    return ans;
}

u64 rho_pow(u64 a, u64 b, u64 mod) {
    u64 ans = 1;
    for (; b; b >>= 1, a = rho_mul(a, a, mod))
        if (b & 1) ans = rho_mul(ans, a, mod);
    return ans;
}

bool is_prime(u64 n) {
    if (n < 2) return 0;
    for (u64 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;
    u64 d = n - 1;
    int s = 0;
    while (!(d & 1)) d >>= 1, s ++;
    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = rho_pow(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool op = 0;
        for (int i = 1; i < s; i ++) {
            x = rho_mul(x, x, n);
            if (x == n - 1) {
                op = 1;
                break;
            }
        }
        if (!op) return 0;
    }
    return 1;
}

u64 rho(u64 n, std::mt19937_64 &rng) {
    if (n % 2 == 0) return 2;
    while (true) {
        u64 c = rng() % (n - 1) + 1, x = rng() % n, y = x, d = 1;
        auto f = [&](u64 v) -> u64 {
            u64 x = rho_mul(v, v, n);
            return x >= n - c ? x - (n - c) : x + c;
        };
        while (d == 1) {
            u64 a = x, b = y, prod = 1;
            for (int i = 0; i < 128; i ++) {
                x = f(x);
                y = f(f(y));
                u64 dif = x > y ? x - y : y - x;
                prod = rho_mul(prod, dif, n);
            }
            d = std::gcd(prod, n);
            if (d == n) {
                x = a;
                y = b;
                do {
                    x = f(x);
                    y = f(f(y));
                    d = std::gcd(x > y ? x - y : y - x, n);
                } while (d == 1);
            }
        }
        if (d != n) return d;
    }
}

std::vector<u64> factor(u64 n) {
    std::vector<u64> ans;
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    auto dfs = [&](auto &&self, u64 x) -> void {
        if (x < 2) return;
        if (is_prime(x)) {
            ans.push_back(x);
            return;
        }
        u64 d = rho(x, rng);
        self(self, d);
        self(self, x / d);
    };
    dfs(dfs, n);
    std::sort(ans.begin(), ans.end());
    return ans;
}

/*
bool prime = is_prime(97); auto factors = factor(84);
u64 prod = rho_mul(12, 34, 97), power = rho_pow(7, 5, 97);
std::mt19937_64 rng(42); u64 divisor = rho(91, rng);
*/
