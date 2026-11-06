#include <bits/stdc++.h>
#include <cassert>

#define V vector

typedef long long ll;

using namespace std;

// FWT：输入数组下标 0-based，长度必须是 2 的幂，元素应先对 mod 归一化。
// 单次变换复杂度 O(n log n)，空间 O(1)；XOR 逆变换要求 mod 为奇质数且 n 在模数下可逆。
// 要求 1 < mod <= 2^31，输入模数值归一化到 [0, mod)，乘加计算不得溢出 ll。

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

// 用法：fwt_or(a, mod);
// ifwt_or(a, mod); // 恢复原数组。
// fwt_xor(a, mod);
// fwt_xor(a, mod, 1);
