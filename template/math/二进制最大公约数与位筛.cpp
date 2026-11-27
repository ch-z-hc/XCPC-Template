/*
无符号整数的二进制 gcd，O(log(max(a,b)+1))。埃氏筛保存 [0,n] 的素数标记，
时间 O(n log log n)，空间约 n 位；n 为非负 int。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
typedef unsigned long long ull;
using namespace std;

ull binary_gcd(ull a, ull b) {
    if (!a || !b) return a | b;
    int shift = __builtin_ctzll(a | b);
    a >>= __builtin_ctzll(a);
    do {
        b >>= __builtin_ctzll(b);
        if (a > b) swap(a, b);
        b -= a;
    } while (b);
    return a << shift;
}

V < bool > prime_flags(int n) {
    V < bool > prime(size_t(n) + 1, true);
    prime[0] = false;
    if (n) prime[1] = false;
    for (int p = 2; p <= n / p; p ++) if (prime[p])
        for (ll j = 1ll * p * p; j <= n; j += p) prime[j] = false;
    return prime;
}

/*
ull d = binary_gcd(48, 18); auto prime = prime_flags(100);
*/
