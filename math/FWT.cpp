#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// FWT：输入数组下标 0-based，长度必须是 2 的幂，元素应先对 mod 归一化。
// 单次变换复杂度 O(n log n)，空间 O(1)；XOR 逆变换要求 n 在 mod 下可逆。
static void fwt_check(const vector<long long> &a) {
    if (a.empty() || (a.size() & (a.size() - 1))) throw invalid_argument("FWT size must be a power of two");
}
static ll fwt_pow(ll a, ll b, ll mod) {
    ll res = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

void fwt_or(vector<long long> &a, long long mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++i)
                a[l + i + len / 2] = (a[l + i + len / 2] + a[l + i]) % mod;
}
void ifwt_or(vector<long long> &a, long long mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++i)
                a[l + i + len / 2] = (a[l + i + len / 2] - a[l + i] + mod) % mod;
}
void fwt_and(vector<long long> &a, long long mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++i)
                a[l + i] = (a[l + i] + a[l + i + len / 2]) % mod;
}
void ifwt_and(vector<long long> &a, long long mod) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++i)
                a[l + i] = (a[l + i] - a[l + i + len / 2] + mod) % mod;
}
void fwt_xor(vector<long long> &a, long long mod, bool inverse = false) {
    fwt_check(a);
    for (int len = 2; len <= int(a.size()); len <<= 1)
        for (int l = 0; l < int(a.size()); l += len)
            for (int i = 0; i < len / 2; ++i) {
                long long x = a[l + i], y = a[l + i + len / 2];
                a[l + i] = (x + y) % mod;
                a[l + i + len / 2] = (x - y + mod) % mod;
            }
    if (inverse) {
        ll inv = fwt_pow(a.size(), mod - 2, mod);
        for (auto &x : a) x = x * inv % mod;
    }
}
