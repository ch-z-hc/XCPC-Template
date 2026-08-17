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
    int n, m, dfstot = 0; cin >> n >> m;
    V < V < int > > nxt(n + 1);
    V < int > dfn(n + 1), low(n + 1), vis(n + 1);
    for (int i = 1; i <= m; ++ i) {
        int u, v; cin >> u >> v;
        nxt[u].push_back(v);
        nxt[v].push_back(u);
    }

    auto tarjan = [&](auto&&self, int u, int rt) -> void {
        int child_size = 0;
        dfn[u] = low[u] = ++ dfstot;
    
        for (auto v : nxt[u]) {
            if (! dfn[v]) {
                child_size ++;
                self(self, v, rt);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    if (u == rt && child_size == 1) continue;
                    vis[u] = 1;
                }
            }
            else low[u] = std::min(low[u], dfn[v]);
        }
    };

    for (int i = 1; i <= n; ++ i)
        if (! dfn[i]) tarjan(tarjan, i, i);

    V < int > ans;

    for (int i = 1; i <= n; ++ i)
        if (vis[i]) ans.push_back(i);

    cout << ans.size() << '\n';

    for (auto x : ans) cout << x << ' ';

    return 0;
}