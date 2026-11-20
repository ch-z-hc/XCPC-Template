#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 模 998244353，系数从常数项开始放，先化到 [0, NTT_MOD)。
// multiply 自动补长，结果长度不能超过 2^23。直接调 ntt 时，数组长度须为 2 的幂且不超过 2^23。
// 设变换长度为 n，时间 O(n log n)。ntt 原地变换，multiply 另用 O(n) 空间。

const ll NTT_MOD = 998244353;
const ll NTT_ROOT = 3;

ll ntt_pow(ll a, ll b) {
    ll res = 1;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % NTT_MOD;
        a = 1ll * a * a % NTT_MOD;
    }
    return res;
}
void ntt(V < ll > &a, bool inv = false) {
    int n = int(a.size());
    for (int i = 1, j = 0; i < n; ++ i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll wn = ntt_pow(NTT_ROOT, (NTT_MOD - 1) / len);
        if (inv) wn = ntt_pow(wn, NTT_MOD - 2);
        for (int l = 0; l < n; l += len) {
            ll w = 1;
            for (int i = 0; i < len / 2; ++ i, w = w * wn % NTT_MOD) {
                ll x = a[l + i], y = w * a[l + i + len / 2] % NTT_MOD;
                a[l + i] = (x + y) % NTT_MOD;
                a[l + i + len / 2] = (x - y + NTT_MOD) % NTT_MOD;
            }
        }
    }
    if (inv) {
        ll inv_n = ntt_pow(n, NTT_MOD - 2);
        for (auto &x : a) x = x * inv_n % NTT_MOD;
    }
}
V < ll > multiply(V < ll > a, V < ll > b) {
    if (a.empty() || b.empty()) return {};
    int need = int(a.size() + b.size() - 1), n = 1;
    while (n < need) n <<= 1;
    a.resize(n); b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; ++ i) a[i] = a[i] * b[i] % NTT_MOD;
    ntt(a, true);
    a.resize(need);
    return a;
}

// V < ll > c = multiply(a, b);
