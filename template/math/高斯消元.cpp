#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef long double ld;

using namespace std;

// 增广矩阵从 0 编号，显式传变量数 m，最后一列（下标 m）是常数。允许 0 行或 0 个变量。
// 返回 0 表示无解，1 表示唯一解，2 表示多解。有解时 x 给出一组解，自由变量取 0。
// 实数版用绝对容差 eps > 0，系数量级悬殊时先缩放。模数版允许负系数，p 须为质数且 <= 2^31。
// 异或版每行用 bitset < N >，只填前 m + 1 位，m < N。第 m 位放方程右侧。
// 设 r = min(n, m)。实数版耗时 O(nmr)，模数版另加 O(r log p)，空间均为 O(nm)。
// 异或版耗时 O(nm + nr ceil(N/w))，空间 O(n ceil(N/w))，w 是机器字长。

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
        for (ll &v : row) {
            v %= p;
            if (v < 0) v += p;
        }
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

// V < ld > x; int status = gauss(a, m, x);
// V < ll > y; int mod_status = gauss_mod(b, m, p, y);
// V < bitset < 3 > > c{3, 6}; // x0 ^ x1 = 0，x1 = 1
// V < int > z; int xor_status = gauss_xor(c, 2, z); // 唯一解 {1, 1}
