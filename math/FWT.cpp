#include <bits/stdc++.h>
using namespace std;

static void fwt_check(const vector<long long> &a) {
    if (a.empty() || (a.size() & (a.size() - 1))) throw invalid_argument("FWT size must be a power of two");
}
static long long fwt_pow(long long a, long long e, long long mod) {
    long long r = 1 % mod;
    for (; e; e >>= 1, a = a * a % mod) if (e & 1) r = r * a % mod;
    return r;
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
        long long inv = fwt_pow(a.size(), mod - 2, mod);
        for (auto &x : a) x = x * inv % mod;
    }
}
