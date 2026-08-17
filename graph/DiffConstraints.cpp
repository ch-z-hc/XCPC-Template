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

const int INF = INT_MAX;

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n, m; cin >> n >> m;

    V < V < pii > > nxt(n + 1);
    V < int > dis(n + 1, INF), vis(n + 1), cnt(n + 1);

    for (int i = 1; i <= m; ++ i) {
        int u, v, w; cin >> u >> v >> w;
        // x_u - x_v <= w
        nxt[v].push_back({u, w});
    }

    for (int i = 1; i <= n; ++ i)
        nxt[0].push_back({i, 0});

    auto spfa = [&]() -> int {
        queue < int > Q; Q.push(0); dis[0] = 0;
    
        while (Q.size()) {
            int u = Q.front(); Q.pop();
            vis[u] = 0;
            if (cnt[u] > n) return 1;
            for (auto [v, w] : nxt[u]) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    if (! vis[v]) Q.push(v), cnt[v] ++;
                }
            }
        }
        return 0;
    };

    if (spfa()) cout << "NO" << '\n';
    else for (int i = 1; i <= n; ++ i)
        cout << dis[i] << ' ';
    return 0;
}