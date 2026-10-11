#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// Cipolla：求 x * x = a (mod p)，p 必须是质数。
// 2 <= p <= 2147483647，a 可为负数；返回较小的非负根。
// 无解返回 nullopt，不检测 p 是否为质数，不支持合数模数。
// 随机选取二次非剩余，期望时间 O(log p)，额外空间 O(1)。

optional < ll > sqrt_mod(ll a, ll p) {
    a %= p;
    if (a < 0) a += p;
    if (! a || p == 2) return a;
    auto qpow = [&](ll x, ll k) {
        ll ans = 1;
        for (; k; k >>= 1, x = x * x % p)
            if (k & 1) ans = ans * x % p;
        return ans;
    };
    if (qpow(a, (p - 1) / 2) != 1) return nullopt;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution < ll > pick(0, p - 1);
    ll b, w;
    do {
        b = pick(rng);
        w = (b * b % p - a + p) % p;
    } while (qpow(w, (p - 1) / 2) != p - 1);

    auto mul = [&](pair < ll, ll > u, pair < ll, ll > v) {
        return pair < ll, ll >{
            (u.first * v.first + u.second * v.second % p * w) % p,
            (u.first * v.second + u.second * v.first) % p
        };
    };
    pair < ll, ll > ans{1, 0}, x{b, 1};
    for (ll k = (p + 1) / 2; k; k >>= 1, x = mul(x, x))
        if (k & 1) ans = mul(ans, x);
    return min(ans.first, p - ans.first);
}

// 调用示例：auto x = sqrt_mod(10, 13); // x = 6。
// sqrt_mod(3, 7) 返回 nullopt；使用前先判断 x.has_value()。
