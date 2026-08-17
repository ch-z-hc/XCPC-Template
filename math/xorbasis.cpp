#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

const int mod = 1e9 + 7;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n; ll ans = 0; cin >> n;
    V < ll > p(64 + 5), a(n + 1);
    for (int i = 1; i <= n; ++ i) cin >> a[i];

    auto insert = [&](ll x) {
        for (int i = 59; i >= 0; -- i)
            if ((x >> i) & 1) {
                if (p[i]) x ^= p[i];
                else { p[i] = x; break; }
            }
    };

    auto getans = [&]() {
        ll ans0 = 0;
        for (int i = 59; i >= 0; -- i)
            if ((ans0 ^ p[i]) > ans0)
                ans0 = ans0 ^ p[i];
        ans = max(ans, ans0);
    };

    auto xorbasis = [&]() {
        for (auto &x : p) x = 0;
        for (int i = 1; i <= n; ++ i)
            insert(a[i]);
        getans();
    };

    xorbasis();

    cout << ans << '\n';

    return 0;
}   