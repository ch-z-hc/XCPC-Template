#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 无权树路径距离：n <= 50；链、菊花、二叉树和随机树，随机重编号。
int main(int argc, char **argv) {
    uint64_t seed = argc > 1 ? stoull(argv[1]) : 1;
    mt19937_64 rng(seed);
    auto rnd = [&](int l, int r) { return uniform_int_distribution < int > (l, r)(rng); };
    int mode = seed % 8;
    int n = mode == 0 ? 1 : rnd(2, 50), q = rnd(1, 80);
    V < int > id(n + 1);
    iota(id.begin(), id.end(), 0); shuffle(id.begin() + 1, id.end(), rng);
    V < pair < int, int > > edges;
    for (int i = 2; i <= n; ++ i) {
        int p = rnd(1, i - 1);
        if (mode == 1) p = i - 1;
        if (mode == 2) p = 1;
        if (mode == 3) p = i / 2;
        int u = id[p], v = id[i];
        if (rnd(0, 1)) swap(u, v);
        edges.push_back({u, v});
    }
    shuffle(edges.begin(), edges.end(), rng);
    cout << n << ' ' << q << ' ' << rnd(1, n) << '\n';
    for (auto [u, v] : edges) cout << u << ' ' << v << '\n';
    for (int i = 1; i <= q; ++ i) {
        int u = rnd(1, n), v = rnd(1, n);
        if (mode == 4) v = u;
        cout << u << ' ' << v << '\n';
    }
}
