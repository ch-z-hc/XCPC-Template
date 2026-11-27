/*
min_root 只求最小原根，无解返回 -1，有解时同时写出欧拉函数 phi。
all_roots 返回正模数 m 的全部原根（升序），无解返回空，m = 1 约定为 {0}。
试除分解与枚举候选，最坏 O(m log² m log log m)，空间 O(m)。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

int min_root(int m, int &phi) {
    phi = 1;
    if (m == 1) return 0;
    auto factors = [](int n) {
        V < int > f;
        for (int d = 2; d <= n / d; d ++) if (n % d == 0) {
            f.push_back(d);
            while (n % d == 0) n /= d;
        }
        if (n > 1) f.push_back(n);
        return f;
    };
    if (m != 2 && m != 4) {
        int t = m % 2 ? m : m / 2;
        if (t % 2 == 0 || factors(t).size() != 1) return -1;
    }
    phi = m;
    for (int p : factors(m)) phi -= phi / p;
    auto f = factors(phi);
    auto power = [m](ll a, int k) {
        ll v = 1;
        for (; k; k >>= 1, a = a * a % m) if (k & 1) v = v * a % m;
        return v;
    };
    int g = 1;
    for (;; g ++) {
        if (gcd(g, m) != 1) continue;
        bool ok = true;
        for (int p : f) if (power(g, phi / p) == 1) ok = false;
        if (ok) return g;
    }
}

V < int > all_roots(int m) {
    int phi, g = min_root(m, phi);
    if (g < 0) return {};
    V < int > ans;
    ll x = 1;
    for (int k = 1; k <= phi; k ++) {
        x = x * g % m;
        if (gcd(k, phi) == 1) ans.push_back(int(x));
    }
    sort(ans.begin(), ans.end());
    return ans;
}

/*
int phi, g = min_root(18, phi); auto roots = all_roots(18);
*/
