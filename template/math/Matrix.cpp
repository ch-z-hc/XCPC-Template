#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 模矩阵乘法与快速幂；矩阵非空、下标 0-based，乘法维度须匹配，快速幂底数须为方阵。
// n 阶方阵乘法 O(n^3)，k 次幂 O(n^3 log k)，结果元素对 mod 取模。
// 快速幂的指数须非负，指数为 0 时返回单位矩阵。
// 要求 1 < mod <= 2^31，输入模数值归一化到 [0, mod)，乘加计算不得溢出 ll。

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

// 调用示例：
// Matrix c = matrix_mul(a, b, mod);
// Matrix p = matrix_pow(a, k, mod);
