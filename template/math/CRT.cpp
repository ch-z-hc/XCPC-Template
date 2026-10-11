#include <bits/stdc++.h>

#define V vector

typedef long long ll;
__extension__ typedef __int128 CRTInt;

using namespace std;

// 扩欧要求 a、b 非负，返回 gcd(a, b)，使 ax + by = gcd(a, b)。
// inv 要求 mod > 0，无逆元时返回 nullopt。
// crt 合并 x = r[i] (mod m[i])，模数不必互质。
// m[i] 必须为正，合并后的 lcm 必须能用 ll 表示；超出时抛 overflow_error。
// 空方程组返回 {0, 1}；结果为 {最小非负解, lcm}，无解返回 nullopt。
// 每次扩欧 O(log M)，CRT 共 O(k log M)，额外空间 O(log M)。

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (! b) { x = 1; y = 0; return a; }
    ll u, v, g = exgcd(b, a % b, u, v);
    x = v;
    y = static_cast<ll> (CRTInt(u) - CRTInt(a / b) * v);
    return g;
}

optional < ll > inv(ll a, ll mod) {
    a %= mod;
    if (a < 0) a += mod;
    ll x, y;
    if (exgcd(a, mod, x, y) != 1) return nullopt;
    return static_cast<ll> ((CRTInt(x) % mod + mod) % mod);
}

optional < pair < ll, ll > > crt(const V < ll > &r, const V < ll > &m) {
    ll ans = 0, mod = 1;
    for (int i = 0; i < int(r.size()); ++ i) {
        ll x, y, g = exgcd(mod, m[i], x, y), q = m[i] / g;
        CRTInt d = CRTInt(r[i]) - ans;
        if (d % g) return nullopt;
        CRTInt k = (d / g % q * x % q + q) % q;
        CRTInt next = CRTInt(mod) * q;
        if (next > LLONG_MAX) throw overflow_error("CRT modulus exceeds ll");
        ans = static_cast<ll> ((CRTInt(ans) + CRTInt(mod) * k) % next);
        mod = static_cast<ll> (next);
    }
    return pair < ll, ll >{ans, mod};
}

// 调用示例：auto ans = crt({2, 3}, {3, 5}); // ans = {8, 15}。
