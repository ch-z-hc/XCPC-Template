#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 数组下标是位掩码，长度 n 须为 2 的幂。系数先化到 [0, mod)，1 < mod <= 2^31。
// OR、AND 的逆变换分别用 ifwt_or、ifwt_and。XOR 逆变换传 true，模数须为奇质数。
// 原地变换，时间 O(n log n)，额外空间 O(1)。

static void fwt_check(const V < ll > &a) {
    assert(! a.empty() && ! (a.size() & (a.size() - 1)));
}
static ll fwt_pow(ll a, ll b, ll mod) {
    ll res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

void fwt_or(V < ll > &a, ll mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++ i)
                a[l + i + len / 2] = (a[l + i + len / 2] + a[l + i]) % mod;
}
void ifwt_or(V < ll > &a, ll mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++ i)
                a[l + i + len / 2] = (a[l + i + len / 2] - a[l + i] + mod) % mod;
}
void fwt_and(V < ll > &a, ll mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++ i)
                a[l + i] = (a[l + i] + a[l + i + len / 2]) % mod;
}
void ifwt_and(V < ll > &a, ll mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++ i)
                a[l + i] = (a[l + i] - a[l + i + len / 2] + mod) % mod;
}
void fwt_xor(V < ll > &a, ll mod, bool inv = false) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++ i) {
                ll x = a[l + i], y = a[l + i + len / 2];
                a[l + i] = (x + y) % mod;
                a[l + i + len / 2] = (x - y + mod) % mod;
            }
    if (inv) {
        ll inv_n = fwt_pow(a.size(), mod - 2, mod);
        for (ll &x : a) x = x * inv_n % mod;
    }
}

// fwt_or(a, mod);
// ifwt_or(a, mod); // 恢复原数组。
// fwt_xor(a, mod);
// fwt_xor(a, mod, 1);
