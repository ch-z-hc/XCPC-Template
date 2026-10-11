#include "../../../../../template/graph/Dijkstra.cpp"

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, m, s; cin >> n >> m >> s; Graph g(n + 1);
    while (m --) {
        int u, v; ll w; cin >> u >> v >> w; g[u].push_back({v, w});
    }
    auto dis = dijkstra(g, s);
    for (int i = 1; i <= n; ++ i) cout << (dis[i] == (1ll << 62) ? -1 : dis[i]) << ' ';
    cout << '\n';
}
