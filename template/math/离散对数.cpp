#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// 求最小非负 x，使 a^x = b (mod mod)，无解返回 -1；约定 a^0 = 1。
// bsgs 要求 gcd(a, mod) = 1；exbsgs 不要求互质。1 <= mod <= 2^31 - 1。
// 时间与空间 O(sqrt(mod))，unordered_map 为期望复杂度。

ll bsgs_inv(ll a, ll mod) {
    auto dfs = [&](auto &&self, ll a_, ll b, ll &x, ll &y) -> void {
        if (! b) { x = 1; y = 0; return; }
        ll u, v;
        self(self, b, a_ % b, u, v);
        x = v; y = u - a_ / b * v;
    };
    ll x, y;
    dfs(dfs, a, mod, x, y);
    return (x % mod + mod) % mod;
}

ll bsgs(ll a, ll b, ll mod) {
    if (mod == 1) return 0;
    a = (a % mod + mod) % mod; b = (b % mod + mod) % mod;
    ll m = ll(sqrtl(mod)) + 1, cur = 1;
    unordered_map < ll, ll > pos;
    pos.reserve(size_t(m * 2));
    for (ll j = 0; j < m; ++ j) {
        pos.emplace(cur, j);
        cur = cur * a % mod;
    }
    ll step = bsgs_inv(cur, mod);
    cur = b;
    for (ll i = 0; i <= m; ++ i) {
        auto it = pos.find(cur);
        if (it != pos.end()) return i * m + it->second;
        cur = cur * step % mod;
    }
    return -1;
}

ll exbsgs(ll a, ll b, ll mod) {
    if (mod == 1) return 0;
    a = (a % mod + mod) % mod; b = (b % mod + mod) % mod;
    ll k = 1, cnt = 0;
    for (ll g; (g = gcd(a, mod)) > 1;) {
        if (b == k) return cnt;
        if (b % g) return -1;
        b /= g; mod /= g; cnt ++;
        k = k * (a / g) % mod;
    }
    ll ans = bsgs(a, b * bsgs_inv(k, mod) % mod, mod);
    return ans == -1 ? -1 : ans + cnt;
}

// 用法：ll x = exbsgs(a, b, mod);
