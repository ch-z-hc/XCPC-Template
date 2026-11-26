/*
生成排列时用 iota(a.begin(), a.end(), 1) 填值，再用 shuffle(a.begin(), a.end(), rng) 打乱。
生成树时，对每个 i = 2..n 连边 (i, rnd(1, i - 1))。
*/

#include <bits/stdc++.h>
using namespace std;

int main() {
    mt19937 rng(random_device{}());
    auto rnd = [&](int l, int r) {
        return uniform_int_distribution < int >(l, r)(rng);
    };
    int n = rnd(1, 10);
    cout << n << '\n';
    for (int i = 1; i <= n; ++ i)
        cout << rnd(1, 100) << (i == n ? '\n' : ' ');
}
