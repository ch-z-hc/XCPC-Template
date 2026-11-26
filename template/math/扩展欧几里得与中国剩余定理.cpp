/*
求 Bézout 系数、模逆和一般同余方程组。exgcd 输入非负，模数为正，各模数可不互质。
合并 k 个方程 O(k log M)，M 为运算中的最大模数。无解或合并模数超出 i64 时失败。
exgcd 给出 ax + by = gcd(a, b) 的系数。inv 无逆元时返回 -1，crt 成功时给出最小非负解和合并模数。
直接调用 crt_mul 时须有 0 <= a, b < mod。
*/

#include <bits/stdc++.h>

using i64 = long long;

i64 exgcd(i64 a, i64 b, i64 &x, i64 &y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    i64 u, v, g = exgcd(b, a % b, u, v);
    x = v;
    y = u - (a / b) * v;
    return g;
}

i64 inv(i64 a, i64 mod) {
    a %= mod;
    if (a < 0) a += mod;
    i64 x, y;
    if (exgcd(a, mod, x, y) != 1) return -1;
    x %= mod;
    return x < 0 ? x + mod : x;
}

i64 crt_mul(i64 a, i64 b, i64 mod) {
    i64 ans = 0;
    for (; b; b >>= 1) {
        if (b & 1) ans = ans >= mod - a ? ans - (mod - a) : ans + a;
        a = a >= mod - a ? a - (mod - a) : a + a;
    }
    return ans;
}

bool crt(const std::vector<i64> &r, const std::vector<i64> &m, i64 &ans, i64 &mod) {
    ans = 0;
    mod = 1;
    for (int i = 0; i < int(r.size()); i ++) {
        i64 x, y, g = exgcd(mod, m[i], x, y), q = m[i] / g;
        i64 rem = r[i] % m[i];
        if (rem < 0) rem += m[i];
        i64 d = rem - ans;
        if (d % g) return false;
        i64 a = d / g % q, b = x % q;
        if (a < 0) a += q;
        if (b < 0) b += q;
        if (mod > LLONG_MAX / q) return false;
        i64 k = crt_mul(a, b, q);
        ans += mod * k;
        mod *= q;
    }
    return true;
}

/*
i64 x, y; i64 g = exgcd(6, 15, x, y);
i64 inverse = inv(3, 5);
i64 ans, mod; bool ok = crt({2, 3}, {3, 5}, ans, mod);
*/
