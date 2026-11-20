#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 矩阵从 0 编号，不能为空。相乘时前者列数等于后者行数，求幂时底数必须是方阵。
// 元素先化到 [0, mod)，1 < mod <= 2^31。指数 k 非负，0 次幂返回单位矩阵。
// n 阶方阵乘法 O(n³)，快速幂 O(n³ log(k + 2))，空间 O(n²)。

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

// Matrix c = matrix_mul(a, b, mod);
// Matrix p = matrix_pow(a, k, mod);
