#include <bits/stdc++.h>

using namespace std;

const int N0 = 1e7 + 5;

int pClock, prime[N0], low[N0];
int f[N0];

void sieve(const int N) {
    f[1] = 1;
    for (int i = 2; i <= N; i ++) {
        if (! low[i]) {
            prime[++ pClock] = i;

            long long x = i; int c = 1;
            while (x <= N) {
                low[x] = x;
                // calculate f(x)
                x *= i, c ++;
            }
        }

        for (int j = 1; j <= pClock; j ++) {
            if (prime[j] > N / i) break;

            int num = i * prime[j]; 

            low[num] = i % prime[j] ? prime[j] : low[i] * prime[j];
            f[num] = f[num / low[num]] * f[low[num]];

            if (i % prime[j] == 0) break;
        }
    }
}


int main() {
    return 0;
}