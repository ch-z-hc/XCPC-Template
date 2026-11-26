// O(n) 筛出 1..n 的质数、最小质因子、欧拉函数和莫比乌斯函数。

#include <bits/stdc++.h>

struct Sieve {
    std::vector<int> prime, lp, phi, mu;
    void build(int n) {
        prime.clear();
        lp.assign(n + 1, 0);
        phi.assign(n + 1, 0);
        mu.assign(n + 1, 0);
        if (n) phi[1] = mu[1] = 1;
        for (int i = 2; i <= n; i ++) {
            if (!lp[i]) {
                lp[i] = i;
                prime.push_back(i);
                phi[i] = i - 1;
                mu[i] = -1;
            }
            for (int p : prime) {
                if (p > lp[i] || 1LL * i * p > n) break;
                lp[i * p] = p;
                if (i % p == 0)
                    phi[i * p] = phi[i] * p, mu[i * p] = 0;
                else
                    phi[i * p] = phi[i] * (p - 1), mu[i * p] = -mu[i];
            }
        }
    }
};

/*
Sieve s; s.build(10);
int lp = s.lp[6], phi = s.phi[6], mu = s.mu[6];
*/
