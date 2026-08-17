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
    int n, m, minV; cin >> n >> m;
    minV = n;
    V < int > pos(n + 1), in(n + 1), out(n + 1), ans;
    V < V < int > > nxt(n + 1);
    for (int i = 1; i <= m; ++ i) {
        int u, v; cin >> u >> v;
        chkmin(minV, min(u, v));
        nxt[u].push_back(v);
        in[v] ++; out[u] ++;
    }

    int cnt = 0;
    for (int i = 1; i <= n; ++ i) {
        if (in[i] != out[i]) { // 若是回路 该处直接 No
            cnt ++;
            if (out[i] > in[i]) minV = i;
        }
        if (abs(in[i] - out[i]) >= 2) {
            cout << "No\n";
            return;
        }
        sort(nxt[i].begin(), nxt[i].end());
    }
    if (cnt > 2) {
        cout << "No\n";
        return;
    }

    auto dfs = [&](auto&&self, int u) -> void {
        while (pos[u] < (int)nxt[u].size()) {
            int v = nxt[u][pos[u]]; pos[u] ++;
            self(self, v);
        }
        ans.push_back(u);
    };

    dfs(dfs, minV);
    reverse(ans.begin(), ans.end());
    for (auto x : ans) cout << x << ' ';
}

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int t = 1; 
    // cin >> t;
    while (t --) solve();
    return 0;
}