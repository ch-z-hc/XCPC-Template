#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

const int mod = 998244353;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

void solve() {
    int n, m; cin >> n >> m;
    V < int > a(n + 1), Log(n + 1);
    V < V < int > > st(n + 1, V < int > (25 + 1));
    Log[0] = -1;
    for (int i = 1; i <= n; ++ i) {
        cin >> a[i];
        st[i][0] = a[i], Log[i] = Log[i >> 1] + 1;
    }

    for (int j = 1; j <= 25; ++j)
        for (int i = 1; i + (1 << (j)) - 1 <= n; ++i)
            st[i][j] = max(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);

    auto ask = [&](int l, int r) {
        int hl = Log[r - l + 1];
        return max(st[l][hl], st[r - (1 << hl) + 1][hl]);
    };

    while (m --) {
        int l, r; cin >> l >> r;
        cout << ask(l, r) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int t = 1;
    // cin >> t;
    while (t --) solve();
    return 0;
}