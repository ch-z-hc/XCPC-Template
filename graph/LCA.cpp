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
    int n, m, s; cin >> n >> m >> s;
    V < V < int > > nxt(n + 1), f(n + 1, V < int > (26));
    V < int > dep(n + 1);

    for (int i = 1; i < n; ++ i) {
        int u, v; cin >> u >> v;
        nxt[u].push_back(v);
        nxt[v].push_back(u);
    }

    auto dfs = [&](auto&&self, int u, int fa) -> void {
        dep[u] = dep[fa] + 1;
        for (auto v : nxt[u]) {
            if (v == fa) continue;
            self(self, v, u);
            f[v][0] = u;
        }
    };

    dfs(dfs, s, 0);

    for (int k = 1; k <= 25; ++ k)
        for (int u = 1; u <= n; ++ u)
            f[u][k] = f[f[u][k - 1]][k - 1];

    auto LCA = [&](int u, int v) {
        if (dep[u] < dep[v])
            swap(u, v);
        for (int i = 25; i >= 0; -- i) {
            if (dep[f[u][i]] >= dep[v])
                u = f[u][i];
            if (u == v) return u;
        }
    
        for (int i = 25; i >= 0; -- i)
            if (f[u][i] != f[v][i])
                u = f[u][i],
                v = f[v][i];
        return f[u][0];
    };

    while (m --) {
        int u, v; cin >> u >> v;
        std::cout << LCA(u, v) << '\n';
    }
    return 0;
}