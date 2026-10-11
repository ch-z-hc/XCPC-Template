#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 1..n 排列，n <= 100；包含有序、逆序、只交换一对和随机排列。
int main(int argc, char **argv) {
    uint64_t seed = argc > 1 ? stoull(argv[1]) : 1;
    mt19937_64 rng(seed);
    auto rnd = [&](int l, int r) { return uniform_int_distribution < int > (l, r)(rng); };
    int mode = seed % 8;
    int n = mode == 0 ? 1 : rnd(1, 100);
    V < int > a(n);
    iota(a.begin(), a.end(), 1);
    if (mode == 2) reverse(a.begin(), a.end());
    else if (mode == 3) swap(a[rnd(0, n - 1)], a[rnd(0, n - 1)]);
    else if (mode != 1) shuffle(a.begin(), a.end(), rng);
    cout << n << '\n';
    for (int i = 0; i < n; ++ i) cout << a[i] << (i + 1 == n ? '\n' : ' ');
}
