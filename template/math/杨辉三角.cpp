/*
按行递推 C(i,j)，适用任意正 int 模数，包括合数。预处理时间和空间 O(n²)，查询 O(1)。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

V < V < int > > pascal(int n, int mod) {
    V < V < int > > c(n + 1);
    for (int i = 0; i <= n; i ++) {
        c[i].assign(i + 1, 1 % mod);
        for (int j = 1; j < i; j ++)
            c[i][j] = (ll(c[i - 1][j - 1]) + c[i - 1][j]) % mod;
    }
    return c;
}

/*
auto c = pascal(100, 1000); int x = c[20][8];
*/
