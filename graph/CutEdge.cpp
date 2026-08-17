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
    int n, m, tot = 1, tim = 0; cin >> n >> m;
    V < int > head(n + 1), nxt(m * 2 + 1), from(m * 2 + 1), to(m * 2 + 1);
    V < int > dfn(n + 1), low(n + 1), bridge(m * 2 + 1);
    auto add = [&](int u, int v) {
        nxt[++ tot] = head[u];
        head[u] = tot;
        from[tot] = u;
        to[tot] = v;
    };

    for (int i = 1; i <= m; ++ i) {
        int u, v; cin >> u >> v;
        add(u, v); add(v, u);
    }

    auto tarjan = [&](auto&&self, int u, int edge) -> void {
        low[u] = dfn[u] = ++ tim;
    
        for (int i = head[u]; i; i = nxt[i]) {
            int v = to[i];
            if (i == (edge ^ 1)) continue;
            if (! dfn[v]) {
                self(self, v, i);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u])
                    bridge[i] = bridge[i ^ 1] = 1;
            }
            else low[u] = min(low[u], dfn[v]);
        }
    };

    for (int i = 1; i <= n; ++ i)
        if (! dfn[i]) tarjan(tarjan, i, 0);


    V < int > ans;

    for (int i = 1; i <= m; ++ i)
        if (bridge[i * 2]) ans.push_back(i);

    cout << ans.size() << '\n';
    for (auto x : ans) cout << x << ' ';

    return 0;
}