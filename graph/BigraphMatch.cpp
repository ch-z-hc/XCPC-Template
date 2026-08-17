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
    int n, m, c, ans = 0; cin >> n >> m >> c;
    V < V < int > > nxt(2 * n + 1);
    V < int > vis(2 * n + 1), match(2 * n + 1);

    auto add = [&](int u, int v) {
        nxt[u].push_back(v);
    };

    for (int i = 1; i <= c; ++ i) {
        int u, v; cin >> u >> v;
        add(u, v + n); 
    }

    auto dfs = [&](auto&&self, int u) -> bool {
        for (auto v : nxt[u]) {
            if (vis[v]) continue;
            vis[v] = 1;
            if (! match[v] || self(self, match[v])) {
                match[v] = u;
                return 1;
            }
        }
        return 0;
    };

    for (int i = 1; i <= n; ++ i) {
        for (auto &x : vis) x = 0;
        if (dfs(dfs, i)) ans ++;
    }

    cout << ans << '\n';
    return 0;
}