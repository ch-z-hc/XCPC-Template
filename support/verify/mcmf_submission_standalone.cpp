#include "../../template/graph/ScalingMinCostFlow.cpp"

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m, s, t;
    if (! (cin >> n >> m >> s >> t)) return 0;

    ScalingMCMF mcf(n);
    for (int i = 1; i <= m; ++ i) {
        int u, v;
        ll cap, cost;
        cin >> u >> v >> cap >> cost;
        mcf.add_edge(u, v, cap, cost);
    }

    auto [flow, cost] = mcf.flow(s, t);
    cout << flow << ' ' << cost << '\n';
    return 0;
}
