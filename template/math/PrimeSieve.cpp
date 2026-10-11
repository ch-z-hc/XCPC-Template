#include <bits/stdc++.h>

#define V vector

using namespace std;

// 线性筛质数；筛数组下标 0..n，返回 V 使用 0-based 存储。
// 复杂度 O(n)，空间 O(n)。

V < int > primes(int n) {
    V < int > prime, lp(n + 1);
    for (int i = 2; i <= n; ++ i) {
        if (! lp[i]) lp[i] = i, prime.push_back(i);
        for (int p : prime) {
            if (p > lp[i] || 1LL * i * p > n) break;
            lp[i * p] = p;
        }
    }
    return prime;
}

// 调用示例：
// V < int > prime = primes(n);
