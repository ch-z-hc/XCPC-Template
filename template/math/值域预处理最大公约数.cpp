/*
对 [0, n] 内整数求最大公约数。筛出三个因子，超过 sqrt(n) 的因子一定为质数。
建表时间和空间 O(n)，每次查询 O(1)。
*/

#include <bits/stdc++.h>

#define V vector
using namespace std;

struct GcdTable {
    int b;
    V < array < int, 3 > > fac;
    V < V < int > > small;

    void init(int n) {
        b = int(sqrt(n));
        V < int > primes, lp(n + 1);
        fac.assign(n + 1, {1, 1, 1});
        for (int i = 2; i <= n; i ++) {
            if (!lp[i]) primes.push_back(lp[i] = i);
            auto a = fac[i / lp[i]];
            a[0] *= lp[i];
            sort(a.begin(), a.end());
            fac[i] = a;
            for (int p : primes) {
                if (p > lp[i] || p > n / i) break;
                lp[i * p] = p;
            }
        }
        small.assign(b + 1, V < int >(b + 1));
        for (int i = 1; i <= b; i ++) {
            small[i][0] = small[0][i] = i;
            for (int j = 1; j <= i; j ++)
                small[j][i] = small[i][j] = small[j][i % j];
        }
    }

    int query(int x, int y) {
        if (!x || !y) return x + y;
        int ans = 1;
        for (int f : fac[y]) {
            int g = f <= b ? small[f][x % f] : (x % f == 0 ? f : 1);
            ans *= g;
            x /= g;
        }
        return ans;
    }
};

/*
GcdTable g; g.init(100000);
int d = g.query(84, 60);
*/
