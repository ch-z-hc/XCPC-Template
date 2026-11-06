#include <bits/stdc++.h>
using namespace std;

// ./gen seed：n 为 1..10，数组元素为 1..100。
int main(int argc, char** argv) {
    if (argc != 2) return 1;
    int seed = stoi(argv[1]);
    mt19937 rng(seed);
    auto rnd = [&](int l, int r) {
        return uniform_int_distribution < int >(l, r)(rng);
    };
    int n = rnd(1, 10);
    cout << n << '\n';
    for (int i = 1; i <= n; ++ i)
        cout << rnd(1, 100) << (i == n ? '\n' : ' ');
    // 字符串：逐位 char('a' + rnd(0, 25))。
    // 排列：iota(a.begin(), a.end(), 1); shuffle(a.begin(), a.end(), rng);
    // 树：对 i = 2..n 连边 (i, rnd(1, i - 1))。
    // 区间：l = rnd(1, n), r = rnd(1, n); 若 l > r 则交换。
}
