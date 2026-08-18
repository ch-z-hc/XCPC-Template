#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 模矩阵乘法与快速幂；矩阵下标 0-based，乘法维度必须匹配，快速幂底数必须为方阵。
// n 阶方阵乘法 O(n^3)，k 次幂 O(n^3 log k)，结果元素对 mod 取模。
using Matrix = vector<vector<long long>>;

Matrix matrix_mul(const Matrix &a, const Matrix &b, long long mod) {
    int n = int(a.size()), m = int(b.size()), p = int(b[0].size());
    Matrix c(n, vector<long long>(p, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < m; ++k) if (a[i][k])
            for (int j = 0; j < p; ++j)
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % mod;
    return c;
}

Matrix matrix_pow(Matrix a, ll b, ll mod) {
    int n = int(a.size());
    Matrix res(n, vector<ll>(n, 0));
    for (int i = 0; i < n; ++i) res[i][i] = 1 % mod;
    for (; b; b >>= 1) {
        if (b & 1) res = matrix_mul(res, a, mod);
        a = matrix_mul(a, a, mod);
    }
    return res;
}

// Usage:
// Matrix c = matrix_mul(a, b, MOD);
// Matrix p = matrix_pow(a, k, MOD);
