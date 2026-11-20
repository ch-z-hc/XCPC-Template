#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// exgcd 对非负 a、b 求 gcd 及满足 ax + by = gcd 的系数。inv 要求模数为正，无逆元返回 -1。
// crt 合并同余式 x = r[i] (mod m[i])。模数须为正，可以不互质。无解或最小公倍数超出 ll 时返回 false。
// 成功时 ans 是最小非负解，mod 是合并后的模数。空方程组返回 ans = 0、mod = 1。
// k 个方程耗时 O(k log M)，其中 M 是运算中最大模数，递归空间 O(log M)。

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (! b) {
        x = 1;
        y = 0;
        return a;
    }
    ll u, v, g = exgcd(b, a % b, u, v);
    x = v; y = u - (a / b) * v;
    return g;
}

ll inv(ll a, ll mod) {
    a %= mod; if (a < 0) a += mod;
    ll x, y;
    if (exgcd(a, mod, x, y) != 1) return -1;
    x %= mod; return x < 0 ? x + mod : x;
}

// 0 <= a, b < mod，mod > 0；倍增模乘，避免 a * b 溢出。
ll crt_mul(ll a, ll b, ll mod) {
    ll ans = 0;
    for (; b; b >>= 1) {
        if (b & 1) ans = ans >= mod - a ? ans - (mod - a) : ans + a;
        a = a >= mod - a ? a - (mod - a) : a + a;
    }
    return ans;
}

bool crt(const V < ll > &r, const V < ll > &m, ll &ans, ll &mod) {
    ans = 0; mod = 1;
    for (int i = 0; i < int(r.size()); ++ i) {
        ll x, y, g = exgcd(mod, m[i], x, y), q = m[i] / g;
        ll rem = r[i] % m[i]; if (rem < 0) rem += m[i];
        ll d = rem - ans;
        if (d % g) return false;
        ll a = d / g % q, b = x % q;
        if (a < 0) a += q;
        if (b < 0) b += q;
        if (mod > LLONG_MAX / q) return false;
        ll k = crt_mul(a, b, q);
        ans += mod * k;
        mod *= q;
    }
    return true;
}

// ll ans, mod;
// bool ok = crt({2, 3}, {3, 5}, ans, mod); // ans = 8，mod = 15
