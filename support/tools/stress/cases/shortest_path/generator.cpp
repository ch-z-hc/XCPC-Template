#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 有向非负权图：n <= 30，m <= 150；包含不连通、重边、自环、零权和大权。
int main(int argc, char **argv) {
    uint64_t seed = argc > 1 ? stoull(argv[1]) : 1;
    mt19937_64 rng(seed);
    auto rnd = [&](int l, int r) { return uniform_int_distribution < int > (l, r)(rng); };
    int mode = seed % 8;
    int n = mode == 0 ? 1 : rnd(2, 30), m = mode == 1 ? 0 : rnd(0, 150);
    V < tuple < int, int, ll > > edges;
    if (mode == 2) {
        for (int i = 1; i < n; ++ i) edges.push_back({i, i + 1, rnd(0, 20)});
    } else for (int i = 0; i < m; ++ i) {
        int u = rnd(1, n), v = rnd(1, n);
        if (mode == 5) u = 1, v = n;
        if (mode == 6) v = u;
        ll w = mode == 3 ? 0 : mode == 4 ? 1000000000ll : rnd(0, 20);
        edges.push_back({u, v, w});
    }
    cout << n << ' ' << edges.size() << ' ' << (mode == 2 ? 1 : rnd(1, n)) << '\n';
    for (auto [u, v, w] : edges) cout << u << ' ' << v << ' ' << w << '\n';
}
