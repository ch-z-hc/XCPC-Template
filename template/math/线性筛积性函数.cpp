#include <bits/stdc++.h>

#define V vector

using namespace std;

// 线性筛：质数表、最小质因子 lp、欧拉函数 phi、莫比乌斯函数 mu。
// 下标 0..n，phi[1] = mu[1] = 1。
// build 复杂度 O(n)，空间 O(n)。

struct Sieve {
    V < int > prime, lp, phi, mu;
    void build(int n) {
        prime.clear(); lp.assign(n + 1, 0);
        phi.assign(n + 1, 0); mu.assign(n + 1, 0);
        if (n) phi[1] = mu[1] = 1;
        for (int i = 2; i <= n; ++ i) {
            if (! lp[i]) {
                lp[i] = i; prime.push_back(i);
                phi[i] = i - 1; mu[i] = -1;
            }
            for (int p : prime) {
                if (p > lp[i] || 1LL * i * p > n) break;
                lp[i * p] = p;
                if (i % p == 0) phi[i * p] = phi[i] * p, mu[i * p] = 0;
                else phi[i * p] = phi[i] * (p - 1), mu[i * p] = -mu[i];
            }
        }
    }
};

// 调用示例：
// Sieve sieve;
// sieve.build(n); // sieve.phi[i]、sieve.mu[i] 分别为 phi(i)、mu(i)。
