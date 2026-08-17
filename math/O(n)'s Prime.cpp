#include <bits/stdc++.h>
using namespace std;

// 线性筛质数；返回不超过 n 的全部质数。
vector<int> linear_primes(int n) {
    vector<int> primes, least(n + 1);
    for (int i = 2; i <= n; ++i) {
        if (!least[i]) least[i] = i, primes.push_back(i);
        for (int p : primes) {
            if (p > least[i] || 1LL * i * p > n) break;
            least[i * p] = p;
        }
    }
    return primes;
}
