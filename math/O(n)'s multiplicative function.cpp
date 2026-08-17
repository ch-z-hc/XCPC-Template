#include <bits/stdc++.h>
using namespace std;

// 线性筛：返回最小质因子和质数表；下标 0..n。
// build 复杂度 O(n)，空间 O(n)。
struct LinearSieve {
    vector<int> primes, least;
    void build(int n) {
        primes.clear(); least.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) {
            if (!least[i]) least[i] = i, primes.push_back(i);
            for (int p : primes) {
                if (p > least[i] || 1LL * i * p > n) break;
                least[i * p] = p;
            }
        }
    }
};
