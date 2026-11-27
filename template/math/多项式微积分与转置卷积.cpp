/*
先复制「多项式基本运算」。系数已按 998244353 取模。微分、常数项为零的积分、
按 x^k-1 折叠系数均为线性时间。相关系数 c[t] = Σ a[t+j] b[j]，用一次卷积。
sparse_power 求 a^k 的前 n 项，要求 a[0] 非零，耗时 O(|a| + n * 非零项数 + log k)。
输入和截断长度 <= 2^21。折叠要求 k > 0。
*/

#include <bits/stdc++.h>

#define V vector
#define pii pair < int, int >

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

V < int > poly_deriv(const V < int > &a) {
    V < int > f;
    for (int i = 1; i < int(a.size()); i ++) f.push_back(1ll * i * a[i] % FPS::mod);
    return f;
}
V < int > poly_integ(const V < int > &a) {
    int mod = FPS::mod;
    V < int > inv(a.size() + 1, 1), f(a.size() + 1);
    for (int i = 1; i <= int(a.size()); i ++) {
        if (i > 1) inv[i] = mod - 1ll * (mod / i) * inv[mod % i] % mod;
        f[i] = 1ll * a[i - 1] * inv[i] % mod;
    }
    return f;
}
V < int > cyclic_fold(const V < int > &a, int k) {
    V < int > f(k);
    for (int i = 0; i < int(a.size()); i ++)
        f[i % k] = (f[i % k] + a[i]) % FPS::mod;
    return f;
}
V < int > poly_corr(const V < int > &a, V < int > b) {
    if (b.empty()) return V < int >(a.size());
    reverse(b.begin(), b.end());
    auto h = FPS::mul(a, b);
    if (h.empty()) return {};
    return V < int >(h.begin() + b.size() - 1, h.end());
}

V < int > sparse_power(const V < int > &a, ull k, int n) {
    if (!n) return {};
    int mod = FPS::mod, inv0 = FPS::power(a[0], mod - 2);
    V < pii > terms;
    for (int j = 1; j < min(n, int(a.size())); j ++)
        if (a[j]) terms.emplace_back(j, 1ll * a[j] * inv0 % mod);
    V < int > f(n), inv(n, 1);
    f[0] = FPS::power(a[0], k % (mod - 1));
    ll exp = (k % mod + 1) % mod;
    for (int i = 1; i < n; i ++) {
        if (i > 1) inv[i] = mod - 1ll * (mod / i) * inv[mod % i] % mod;
        ll v = 0;
        for (auto [j, w] : terms) {
            if (j > i) break;
            ll c = (exp * j - i + mod) % mod;
            v = (v + c * w % mod * f[i - j]) % mod;
        }
        f[i] = v * inv[i] % mod;
    }
    return f;
}

/*
auto d = poly_deriv({1, 2, 3}), a = poly_integ(d);
auto f = cyclic_fold({1, 2, 3}, 2), g = poly_corr({1, 2, 3}, {2, 1});
auto h = sparse_power({1, 0, 1}, 100, 50);
*/
