#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef long double ld;

using namespace std;

// 高斯消元：增广矩阵 0-based，n 行，每行 m + 1 列，最后一列为常数项。
// 显式传变量数 m >= 0，允许 n = 0 或 m = 0；矩阵按值传入，不改变原矩阵。
// 返回 0 无解 / 1 唯一解 / 2 多解；有解时 x 长度为 m，自由变量置 0。
// 实数版要求 eps > 0，按绝对 eps 判断主元与矛盾行，病态矩阵需缩放。
// 模数版要求 p 为质数且 2 <= p <= 2^31，输入允许负数；无解时 x 不可用。
// 异或版第 m 位为常数，0 <= m < N；输入只使用前 m + 1 位。
// 实数版 O(n m min(n, m))；模数版另有 O(min(n, m) log p) 的求逆开销。
// 实数 / 模数版空间 O(n m)。
// 异或版 O(n m + n min(n, m) ceil(N / w))，空间 O(n ceil(N / w))，w 为机器字长。

int gauss(V < V < ld > > a, int m, V < ld > &x, ld eps = 1e-12L) {
    int n = int(a.size()), r = 0;
    V < int > pos(m, -1);
    x.assign(m, 0);
    for (int j = 0; j < m && r < n; ++ j) {
        int p = r;
        for (int i = r + 1; i < n; ++ i)
            if (abs(a[i][j]) > abs(a[p][j])) p = i;
        if (abs(a[p][j]) <= eps) continue;
        swap(a[r], a[p]);
        ld v = a[r][j];
        for (int k = j; k <= m; ++ k) a[r][k] /= v;
        for (int i = 0; i < n; ++ i) if (i != r) {
            ld t = a[i][j];
            for (int k = j + 1; k <= m; ++ k) a[i][k] -= t * a[r][k];
            a[i][j] = 0;
        }
        pos[j] = r ++;
    }
    for (int i = r; i < n; ++ i)
        if (abs(a[i][m]) > eps) return 0;
    for (int j = 0; j < m; ++ j)
        if (pos[j] != -1) x[j] = a[pos[j]][m];
    return r == m ? 1 : 2;
}

int gauss_mod(V < V < ll > > a, int m, ll p, V < ll > &x) {
    int n = int(a.size()), r = 0;
    V < int > pos(m, -1);
    x.assign(m, 0);
    for (auto &row : a)
        for (ll &v : row) { v %= p; if (v < 0) v += p; }
    auto power = [&](ll a, ll b) {
        ll ans = 1;
        for (; b; b >>= 1, a = a * a % p)
            if (b & 1) ans = ans * a % p;
        return ans;
    };
    for (int j = 0; j < m && r < n; ++ j) {
        int k = r;
        while (k < n && a[k][j] == 0) k ++;
        if (k == n) continue;
        swap(a[r], a[k]);
        ll inv = power(a[r][j], p - 2);
        for (int k = j; k <= m; ++ k) a[r][k] = a[r][k] * inv % p;
        for (int i = 0; i < n; ++ i) if (i != r && a[i][j]) {
            ll t = a[i][j];
            for (int k = j + 1; k <= m; ++ k)
                a[i][k] = (a[i][k] - t * a[r][k] % p + p) % p;
            a[i][j] = 0;
        }
        pos[j] = r ++;
    }
    for (int i = r; i < n; ++ i) if (a[i][m]) return 0;
    for (int j = 0; j < m; ++ j)
        if (pos[j] != -1) x[j] = a[pos[j]][m];
    return r == m ? 1 : 2;
}

template < size_t N >
int gauss_xor(V < bitset < N > > a, int m, V < int > &x) {
    int n = int(a.size()), r = 0;
    V < int > pos(m, -1);
    x.assign(m, 0);
    for (int j = 0; j < m && r < n; ++ j) {
        int k = r;
        while (k < n && ! a[k][j]) k ++;
        if (k == n) continue;
        swap(a[r], a[k]);
        for (int i = 0; i < n; ++ i)
            if (i != r && a[i][j]) a[i] ^= a[r];
        pos[j] = r ++;
    }
    for (int i = r; i < n; ++ i) if (a[i][m]) return 0;
    for (int j = 0; j < m; ++ j)
        if (pos[j] != -1) x[j] = int(a[pos[j]][m]);
    return r == m ? 1 : 2;
}

// 调用示例：
// V < ld > x; int op = gauss(a, m, x);
// V < ll > y; int op = gauss_mod(b, m, p, y);
// 异或示例：x0 ^ x1 = 1，x1 = 0；N 必须为编译期常量，且 N > m。
// const int N = 1005; int n = 2, m = 2;
// V < bitset < N > > c(n);
// c[0][0] = c[0][1] = 1; c[0][m] = 1;
// c[1][1] = 1; c[1][m] = 0;
// V < int > z; int op = gauss_xor(c, m, z); // op = 1，z = {1, 0}。
