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

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n, m, s; cin >> n >> m >> s;
    V < V < pii > > nxt(n + 1);
    V < int > dis(n + 1, INF), vis(n + 1);
    priority_queue < pii > Q;

    for (int i = 1; i <= m; ++ i) {
        int u, v, w; cin >> u >> v >> w;
        nxt[u].push_back({v, w});
    }

    dis[s] = 0; Q.push({0, s});

    while (Q.size()) {
        int u = Q.top().second; Q.pop();
        if (vis[u]) continue;
        vis[u] = 1;

        for (auto [v, w] : nxt[u]) {
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (! vis[v]) 
                    Q.push(std::make_pair(-dis[v], v));
            }
        }
    }

    for (int i = 1; i <= n; ++ i)   
        cout << dis[i] << ' ';
    return 0;
}