#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define V vector

const ll p = 998244353, g = 3, gi = 332748118;

void prev(int & lim, int n, V < int > &r) {
    int l = 0;
    lim = 1; while (lim <= n) lim <<= 1, l ++;
    r.resize(lim);
    for (int i = 0; i < lim; i ++)
        r[i] = (r[i >> 1] >> 1) | ((i & 1) << (l - 1));
}

int quick_pow(ll a, ll b) {
    ll res = 1;
    for (; b; b >>= 1) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
    }
    return res;
}

void NTT(V < ll > &A, V < int > &r, int lim, int type) {
    for (int i = 0; i < lim; i ++) 
        if (i < r[i]) swap(A[i], A[r[i]]);
    for (int mid = 1; mid < lim; mid <<= 1) {
        ll Wn = quick_pow(type == 1 ? g : gi, (p - 1) / (mid << 1));
        for (int R = mid << 1, j = 0; j < lim; j += R) {
            ll w = 1;
            for (int k = 0; k < mid; k ++, w = w * Wn % p) {
                ll x = A[j + k], y = w * A[j + mid + k] % p;
                A[j + k] = (x + y) % p; A[j + mid + k] = (x - y + p) % p;
            }
        }
    }
    if (type == 1) return;
    ll inv = quick_pow(lim, p - 2);
    for (int i = 0; i < lim; i ++)
        A[i] = A[i] * inv % p;
}

V < ll > mulFG(V < ll > F, V < ll > G) {
    int n = F.size() - 1, m = G.size() - 1;
    V < int > r; int lim;
    prev(lim, n + m, r);
    F.resize(lim); G.resize(lim);
    NTT(F, r, lim, 1);
    NTT(G, r, lim, 1);
    for (int i = 0; i < lim; ++ i)
        F[i] = F[i] * G[i] % p;
    NTT(F, r, lim, -1);
    F.resize(n + m + 1);
    return F;
}

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    V < ll > F(n + 1), G(m + 1);
    for (int i = 0; i <= n; ++ i) cin >> F[i];
    for (int i = 0; i <= m; ++ i) cin >> G[i];
    F = mulFG(F, G);
    for (int i = 0; i <= n + m; ++ i)
        cout << F[i] << ' ';
    return 0;
}