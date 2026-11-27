#include <bits/stdc++.h>

#define V vector
using namespace std;

V < int > primes(int n) {
    V < int > prime, lp(n + 1);
    for (int i = 2; i <= n; i ++) {
        if (!lp[i]) lp[i] = i, prime.push_back(i);
        for (int p : prime) {
            if (p > lp[i] || p > n / i) break;
            lp[i * p] = p;
        }
    }
    return prime;
}

// V < int > prime = primes(n);
