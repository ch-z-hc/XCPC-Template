#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 区间加 / 区间和：n <= 40，q <= 80；包含单点、全区间、负数和零。
int main(int argc, char **argv) {
    uint64_t seed = argc > 1 ? stoull(argv[1]) : 1;
    mt19937_64 rng(seed);
    auto rnd = [&](int l, int r) { return uniform_int_distribution < int > (l, r)(rng); };
    int mode = seed % 8;
    int n = mode == 0 ? 1 : rnd(1, 40), q = rnd(1, 80);
    cout << n << ' ' << q << '\n';
    for (int i = 1; i <= n; ++ i) {
        int x = rnd(-100, 100);
        if (mode == 2) x = 0;
        if (mode == 3) x = abs(x);
        if (mode == 4) x = -abs(x);
        if (mode == 5) x = 7;
        cout << x << (i == n ? '\n' : ' ');
    }
    for (int i = 1; i <= q; ++ i) {
        int op = i == 1 || i == q ? 1 : rnd(0, 1);
        int l = rnd(1, n), r = rnd(1, n);
        if (l > r) swap(l, r);
        if (mode == 6) l = 1, r = n;
        cout << op << ' ' << l << ' ' << r;
        if (! op) cout << ' ' << rnd(-100, 100);
        cout << '\n';
    }
}
