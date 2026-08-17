#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << ' '
#define V vector
#define fi first
#define se second
#define pii pair < ll, int >

typedef long long ll;
typedef long double ld;

const ld eps = 1e-7;
const int mod = 1e9 + 7;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void mul(T &x, T y) { x = 1ll * x * y % mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

void solve() {
    int n, m; cin >> n >> m;
    int st = n, minV = n;
    V < int > pos(n + 1), deg(n + 1), ans;
    V < V < pair < int, int > > > nxt(n + 1);
    V < int > used(m + 1, 0);

    for (int i = 1; i <= m; ++ i) {
        int u, v; cin >> u >> v;
        nxt[u].push_back({v, i});
        nxt[v].push_back({u, i});
        chkmin(minV, min(u, v));
        deg[u] ++; deg[v] ++;
    }

    int cnt = 0;
    for (int i = 1; i <= n; ++ i) {
        if (deg[i] & 1) { // 若是回路 该处直接 No
            cnt ++;
            chkmin(st, i);
        }
        sort(nxt[i].begin(), nxt[i].end());
    }
    if (cnt > 2) {
        cout << "No\n";
        return;
    }
    if (cnt == 0) st = minV;

    auto dfs = [&](auto&& self, int u) -> void {
        while (pos[u] < (int)nxt[u].size()) {
            auto [v, id] = nxt[u][pos[u]];
            pos[u] ++;
            if (used[id]) continue;
            used[id] = 1;
            self(self, v);
        }
        ans.push_back(u);
    };

    dfs(dfs, st);

    reverse(ans.begin(), ans.end());
    for (auto x : ans) cout << x << '\n';
}

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int t = 1; 
    // cin >> t;
    while (t --) solve();
    return 0;
}
