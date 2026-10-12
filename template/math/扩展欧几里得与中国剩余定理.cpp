#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 扩欧要求 a、b 非负，返回 gcd(a, b)，使 ax + by = gcd(a, b)。
// inv 要求 mod > 0，无逆元时返回 -1。
// crt 合并 x = r[i] (mod m[i])，模数不必互质。
// m[i] 必须为正；crt 无解或合并后的 lcm 超出 ll 范围时返回 false。
// 成功时 ans、mod 为最小非负解和 lcm；空方程组为 0、1。
// 每次扩欧 O(log M)，CRT 共 O(k log M)，额外空间 O(log M)。

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (! b) { x = 1; y = 0; return a; }
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
        ll k = crt_mul(a, b, q); ans += mod * k; mod *= q;
    }
    return true;
}

// 调用示例：ll ans, mod; bool ok = crt({2, 3}, {3, 5}, ans, mod);
// 仅在 ok 为 true 时使用输出；此例 ans = 8，mod = 15。
