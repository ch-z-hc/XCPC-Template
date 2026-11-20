#include <bits/stdc++.h>

#define V vector

using namespace std;

// primes(n) 返回不超过 n 的所有质数，按从小到大排列。时间和空间 O(n)。

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

// V < int > prime = primes(n);
