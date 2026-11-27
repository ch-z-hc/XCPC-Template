/*
固定底数与正 int 模数，预处理 [0, n] 的指数。建表时间和空间 O(sqrt(n))，单次求幂 O(1)。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

struct FixedPower {
    int b, mod;
    V < int > small, large;

    void init(ll a, int n, int m) {
        mod = m;
        b = int(sqrt(n)) + 1;
        a = (a % m + m) % m;
        small.assign(b + 1, 1 % m); large.assign(n / b + 1, 1 % m);
        for (int i = 1; i <= b; i ++) small[i] = small[i - 1] * a % m;
        for (int i = 1; i <= n / b; i ++)
            large[i] = 1ll * large[i - 1] * small[b] % m;
    }

    int pow(int k) {
        return 1ll * large[k / b] * small[k % b] % mod;
    }
};

/*
FixedPower f; f.init(-2, 1000000, 998244353);
int x = f.pow(12345);
*/
