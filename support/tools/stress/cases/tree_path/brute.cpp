#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 每次询问从起点 BFS，不依赖 LCA，O(nq)。
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, q, root; cin >> n >> q >> root; (void)root;
    V < V < int > > g(n + 1);
    for (int i = 1; i < n; ++ i) {
        int u, v; cin >> u >> v; g[u].push_back(v); g[v].push_back(u);
    }
    while (q --) {
        int s, t; cin >> s >> t;
        V < int > dis(n + 1, -1); queue < int > que;
        dis[s] = 0; que.push(s);
        while (! que.empty()) {
            int u = que.front(); que.pop();
            for (int v : g[u]) if (dis[v] == -1) {
                dis[v] = dis[u] + 1; que.push(v);
            }
        }
        cout << dis[t] << '\n';
    }
}
