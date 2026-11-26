#include <bits/stdc++.h>

std::vector<int> primes(int n) {
    std::vector<int> prime, lp(n + 1);
    for (int i = 2; i <= n; i ++) {
        if (!lp[i]) lp[i] = i, prime.push_back(i);
        for (int p : prime) {
            if (p > lp[i] || p > n / i) break;
            lp[i * p] = p;
        }
    }
    return prime;
}

// std::vector<int> prime = primes(n);
