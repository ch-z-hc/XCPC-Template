#include <bits/stdc++.h>
using namespace std;

// 线性筛质数；筛数组下标 0..n，返回 vector 使用 0-based 存储。
// 复杂度 O(n)，空间 O(n)。
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
