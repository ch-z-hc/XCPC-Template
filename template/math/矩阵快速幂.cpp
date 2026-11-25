#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
矩阵乘法要求维数相容，快速幂要求非空方阵、指数非负。元素先取模，1 < mod <= 2^31。
n 阶方阵乘法 O(n³)，快速幂 O(n³ log(k + 2))。
*/

typedef V < V < ll > > Matrix;

Matrix matrix_mul(const Matrix &a, const Matrix &b, ll mod) {
    int n = int(a.size()), m = int(b.size()), p = int(b[0].size());
    Matrix c(n, V < ll > (p, 0));
    for (int i = 0; i < n; ++ i)
        for (int k = 0; k < m; ++ k) if (a[i][k])
            for (int j = 0; j < p; ++ j)
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % mod;
    return c;
}

Matrix matrix_pow(Matrix a, ll b, ll mod) {
    int n = int(a.size());
    Matrix res(n, V < ll > (n, 0));
    for (int i = 0; i < n; ++ i) res[i][i] = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = matrix_mul(res, a, mod);
        a = matrix_mul(a, a, mod);
    }
    return res;
}

/*
Matrix c = matrix_mul(a, b, mod);
Matrix p = matrix_pow(a, k, mod);
*/
