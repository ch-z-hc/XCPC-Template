#include <bits/stdc++.h>
using namespace std;

// NTT：固定模数 998244353，数组下标 0-based，长度必须是 2 的幂且不超过 2^23。
// 输入系数应先归一化到 [0, NTT_MOD)；复杂度 O(n log n)，空间 O(1)。
using NTTInt = long long;
constexpr NTTInt NTT_MOD = 998244353;
constexpr NTTInt NTT_ROOT = 3;

NTTInt ntt_pow(NTTInt a, NTTInt e) {
    NTTInt r = 1;
    for (; e; e >>= 1, a = a * a % NTT_MOD) if (e & 1) r = r * a % NTT_MOD;
    return r;
}
void ntt(vector<NTTInt> &a, bool inverse) {
    int n = int(a.size());
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        NTTInt wn = ntt_pow(NTT_ROOT, (NTT_MOD - 1) / len);
        if (inverse) wn = ntt_pow(wn, NTT_MOD - 2);
        for (int l = 0; l < n; l += len) {
            NTTInt w = 1;
            for (int i = 0; i < len / 2; ++i, w = w * wn % NTT_MOD) {
                NTTInt x = a[l + i], y = w * a[l + i + len / 2] % NTT_MOD;
                a[l + i] = (x + y) % NTT_MOD;
                a[l + i + len / 2] = (x - y + NTT_MOD) % NTT_MOD;
            }
        }
    }
    if (inverse) {
        NTTInt inv_n = ntt_pow(n, NTT_MOD - 2);
        for (auto &x : a) x = x * inv_n % NTT_MOD;
    }
}
vector<NTTInt> polynomial_multiply(vector<NTTInt> a, vector<NTTInt> b) {
    if (a.empty() || b.empty()) return {};
    int need = int(a.size() + b.size() - 1), n = 1;
    while (n < need) n <<= 1;
    a.resize(n); b.resize(n); ntt(a, false); ntt(b, false);
    for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % NTT_MOD;
    ntt(a, true); a.resize(need); return a;
}
