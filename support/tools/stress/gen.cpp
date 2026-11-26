/*
生成排列时用 std::iota(a.begin(), a.end(), 1) 填值，
再用 std::shuffle(a.begin(), a.end(), rng) 打乱。
生成树时，对每个 i = 2..n 连边 (i, rnd(1, i - 1))。
*/

#include <bits/stdc++.h>

std::mt19937_64 rng{std::random_device{}()};

int rnd(int l, int r) {
    return std::uniform_int_distribution<int>(l, r)(rng);
}

int main() {
    int n = rnd(1, 10);
    std::cout << n << '\n';
    for (int i = 1; i <= n; i ++) {
        std::cout << rnd(1, 100) << (i == n ? '\n' : ' ');
    }
}
