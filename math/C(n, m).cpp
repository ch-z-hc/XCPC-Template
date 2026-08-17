#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << ' '

using namespace std;

const int N0 = 1e6;
const int mod = 998244353;
int frac[N0 + 5], invfrac[N0 + 5];

int quick_pow(int a, int b) {
    int res = 1;
    for (; b; b >>= 1) {
        if (b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
    }
    return res;
}

void sieve() {
    frac[0] = invfrac[0] = 1;
    for (int i = 1; i <= N0; ++ i)
        frac[i] = 1ll * frac[i - 1] * i % mod;
    invfrac[N0] = quick_pow(frac[N0], mod - 2);
    for (int i = N0 - 1; i >= 1; -- i)
        invfrac[i] = 1ll * invfrac[i + 1] * (i + 1) % mod;
}

int C(int n, int m) {
    if (n < m || n < 0 || m < 0) return 0;
    return 1ll * frac[n] * invfrac[m] % mod * invfrac[n - m] % mod;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    sieve();
    return 0;
}