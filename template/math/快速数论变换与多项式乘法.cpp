/*
固定模 998244353，低次系数在前且已取模。变换长度须为 2 的幂且 <= 2^23，卷积自动补长。
变换和卷积均为 O(n log n)，n 为补长后的长度。单位根按需缓存，空间 O(最大变换长度)。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

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
    if (inv && n > 1) reverse(a.begin() + 1, a.end());
    for (int i = 1, j = 0; i < n; i ++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    static V < ll > roots{0, 1};
    for (int k = int(roots.size()); k < n; k *= 2) {
        ll w = ntt_pow(NTT_ROOT, (NTT_MOD - 1) / (2 * k));
        roots.resize(2 * k);
        for (int j = k / 2; j < k; j ++) {
            roots[2 * j] = roots[j];
            roots[2 * j + 1] = 1ll * roots[j] * w % NTT_MOD;
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int l = 0; l < n; l += 2 * k) {
            for (int j = 0; j < k; j ++) {
                ll x = a[l + j], y = 1ll * a[l + k + j] * roots[k + j] % NTT_MOD;
                a[l + j] = x + y;
                if (a[l + j] >= NTT_MOD) a[l + j] -= NTT_MOD;
                a[l + k + j] = x - y;
                if (a[l + k + j] < 0) a[l + k + j] += NTT_MOD;
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
    ntt(a, false); ntt(b, false);
    for (int i = 0; i < n; i ++) a[i] = a[i] * b[i] % NTT_MOD;
    ntt(a, true);
    a.resize(need);
    return a;
}

/*
V < ll > a{1, 2, 0, 0}; ntt(a); ntt(a, true);
auto c = multiply({1, 2}, {3, 4}); ll inv2 = ntt_pow(2, NTT_MOD - 2);
*/
