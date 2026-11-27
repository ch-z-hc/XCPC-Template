/*
支持整个 ull 范围。确定性判素用 O(log n) 次模乘；Pollard–Rho 找因子期望约 O(sqrt(p)) 次模运算，
p 为最小质因子。一次倍增模乘为 O(log n)。
factor 对 n < 2 返回空表，其余按非降序返回质因子，保留重数。直接调用 rho 时须传入合数。
*/

#include <bits/stdc++.h>

#define V vector

typedef unsigned long long ull;

using namespace std;

ull rho_mul(ull a, ull b, ull mod) {
    a %= mod;
    ull ans = 0;
    for (; b; b >>= 1) {
        if (b & 1) ans = ans >= mod - a ? ans - (mod - a) : ans + a;
        a = a >= mod - a ? a - (mod - a) : a + a;
    }
    return ans;
}

ull rho_pow(ull a, ull b, ull mod) {
    ull ans = 1;
    for (; b; b >>= 1, a = rho_mul(a, a, mod))
        if (b & 1) ans = rho_mul(ans, a, mod);
    return ans;
}

bool is_prime(ull n) {
    if (n < 2) return 0;
    for (ull p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;
    ull d = n - 1;
    int s = 0;
    while (!(d & 1)) d >>= 1, s ++;
    for (ull a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        ull x = rho_pow(a % n, d, n);
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

ull rho(ull n, mt19937_64 &rng) {
    if (n % 2 == 0) return 2;
    while (true) {
        ull c = rng() % (n - 1) + 1, x = rng() % n, y = x, d = 1;
        auto f = [&](ull v) -> ull {
            ull x = rho_mul(v, v, n);
            return x >= n - c ? x - (n - c) : x + c;
        };
        while (d == 1) {
            ull a = x, b = y, prod = 1;
            for (int i = 0; i < 128; i ++) {
                x = f(x);
                y = f(f(y));
                ull dif = x > y ? x - y : y - x;
                prod = rho_mul(prod, dif, n);
            }
            d = gcd(prod, n);
            if (d == n) {
                x = a;
                y = b;
                do {
                    x = f(x);
                    y = f(f(y));
                    d = gcd(x > y ? x - y : y - x, n);
                } while (d == 1);
            }
        }
        if (d != n) return d;
    }
}

V < ull > factor(ull n) {
    V < ull > ans;
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    auto dfs = [&](auto &&self, ull x) -> void {
        if (x < 2) return;
        if (is_prime(x)) {
            ans.push_back(x);
            return;
        }
        ull d = rho(x, rng);
        self(self, d);
        self(self, x / d);
    };
    dfs(dfs, n);
    sort(ans.begin(), ans.end());
    return ans;
}

/*
bool prime = is_prime(97); auto factors = factor(84);
ull prod = rho_mul(12, 34, 97), power = rho_pow(7, 5, 97);
mt19937_64 rng(42); ull divisor = rho(91, rng);
*/
