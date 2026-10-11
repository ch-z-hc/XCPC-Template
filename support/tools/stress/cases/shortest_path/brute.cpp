#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// Bellman-Ford，O(nm)；不可达输出 -1。
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m, s; cin >> n >> m >> s;
    V < tuple < int, int, ll > > edges(m);
    for (auto &[u, v, w] : edges) cin >> u >> v >> w;
    const ll inf = 1ll << 62; V < ll > dis(n + 1, inf); dis[s] = 0;
    for (int i = 1; i < n; ++ i) {
        bool op = 0;
        for (auto [u, v, w] : edges) if (dis[u] != inf && dis[v] > dis[u] + w) {
            dis[v] = dis[u] + w; op = 1;
        }
        if (! op) break;
    }
    for (int i = 1; i <= n; ++ i) cout << (dis[i] == inf ? -1 : dis[i]) << ' ';
    cout << '\n';
}
