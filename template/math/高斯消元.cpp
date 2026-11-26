/*
求 n 行、m 个未知量的线性方程组，增广矩阵第 m 列是右侧，均从 0 编号。
实数用绝对容差 eps > 0，模数版须用质数 p <= 2^31；异或版要求 m < N，只填 bitset 前 m + 1 位。
r = min(n, m)，实数 O(nmr)，模数版另加 O(r log p)，异或 O(nm + nr ceil(N/w))，w 为机器字长。
三种接口均返回 0 无解、1 唯一解、2 多解，有解时输出一组解。
*/

#include <bits/stdc++.h>

using i64 = long long;
using ld = long double;

int gauss(std::vector<std::vector<ld>> a, int m, std::vector<ld> &x, ld eps = 1e-12L) {
    int n = int(a.size()), r = 0;
    std::vector<int> pos(m, -1);
    x.assign(m, 0);
    for (int j = 0; j < m && r < n; j ++) {
        int p = r;
        for (int i = r + 1; i < n; i ++)
            if (std::abs(a[i][j]) > std::abs(a[p][j])) p = i;
        if (std::abs(a[p][j]) <= eps) continue;
        std::swap(a[r], a[p]);
        ld v = a[r][j];
        for (int k = j; k <= m; k ++) a[r][k] /= v;
        for (int i = 0; i < n; i ++)
            if (i != r) {
                ld t = a[i][j];
                for (int k = j + 1; k <= m; k ++) a[i][k] -= t * a[r][k];
                a[i][j] = 0;
            }
        pos[j] = r ++;
    }
    for (int i = r; i < n; i ++)
        if (std::abs(a[i][m]) > eps) return 0;
    for (int j = 0; j < m; j ++)
        if (pos[j] != -1) x[j] = a[pos[j]][m];
    return r == m ? 1 : 2;
}

int gauss_mod(std::vector<std::vector<i64>> a, int m, i64 p, std::vector<i64> &x) {
    int n = int(a.size()), r = 0;
    std::vector<int> pos(m, -1);
    x.assign(m, 0);
    for (auto &row : a)
        for (i64 &v : row) {
            v %= p;
            if (v < 0) v += p;
        }
    auto power = [&](i64 a, i64 b) {
        i64 ans = 1;
        for (; b; b >>= 1, a = a * a % p)
            if (b & 1) ans = ans * a % p;
        return ans;
    };
    for (int j = 0; j < m && r < n; j ++) {
        int k = r;
        while (k < n && a[k][j] == 0) k ++;
        if (k == n) continue;
        std::swap(a[r], a[k]);
        i64 inv = power(a[r][j], p - 2);
        for (int k = j; k <= m; k ++) a[r][k] = a[r][k] * inv % p;
        for (int i = 0; i < n; i ++)
            if (i != r && a[i][j]) {
                i64 t = a[i][j];
                for (int k = j + 1; k <= m; k ++)
                    a[i][k] = (a[i][k] - t * a[r][k] % p + p) % p;
                a[i][j] = 0;
            }
        pos[j] = r ++;
    }
    for (int i = r; i < n; i ++)
        if (a[i][m]) return 0;
    for (int j = 0; j < m; j ++)
        if (pos[j] != -1) x[j] = a[pos[j]][m];
    return r == m ? 1 : 2;
}

template <std::size_t N>
int gauss_xor(std::vector<std::bitset<N>> a, int m, std::vector<int> &x) {
    int n = int(a.size()), r = 0;
    std::vector<int> pos(m, -1);
    x.assign(m, 0);
    for (int j = 0; j < m && r < n; j ++) {
        int k = r;
        while (k < n && !a[k][j]) k ++;
        if (k == n) continue;
        std::swap(a[r], a[k]);
        for (int i = 0; i < n; i ++)
            if (i != r && a[i][j]) a[i] ^= a[r];
        pos[j] = r ++;
    }
    for (int i = r; i < n; i ++)
        if (a[i][m]) return 0;
    for (int j = 0; j < m; j ++)
        if (pos[j] != -1) x[j] = int(a[pos[j]][m]);
    return r == m ? 1 : 2;
}

/*
std::vector<ld> x; int status = gauss(a, m, x);
std::vector<i64> y; int mod_status = gauss_mod(b, m, p, y);
std::vector<std::bitset<3>> c{3, 6};
std::vector<int> z; int xor_status = gauss_xor(c, 2, z);
*/
