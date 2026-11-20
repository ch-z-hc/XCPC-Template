#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// 求满足 a^x 与 b 模 mod 同余的最小非负 x，无解返回 -1。约定 a^0 = 1。
// a 与 mod 互质时可用 bsgs，一般情况用 exbsgs。1 <= mod <= INT_MAX。
// 使用哈希表，期望时间和空间 O(sqrt(mod))。

ll bsgs_inv(ll a, ll mod) {
    auto dfs = [&](auto &&self, ll a_, ll b, ll &x, ll &y) -> void {
        if (! b) {
            x = 1;
            y = 0;
            return;
        }
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
        b /= g;
        mod /= g;
        cnt ++;
        k = k * (a / g) % mod;
    }
    ll ans = bsgs(a, b * bsgs_inv(k, mod) % mod, mod);
    return ans == -1 ? -1 : ans + cnt;
}

// ll x = exbsgs(a, b, mod);
