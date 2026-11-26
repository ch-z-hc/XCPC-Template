/*
求 n 行、m 个未知量的线性方程组，增广矩阵第 m 列是右侧，均从 0 编号。
实数用绝对容差 eps > 0，模数版须用质数 p <= 2^31；异或版要求 m < N，只填 bitset 前 m + 1 位。
r = min(n, m)，实数 O(nmr)，模数版另加 O(r log p)，异或 O(nm + nr ceil(N/w))，w 为机器字长。
三种接口均返回 0 无解、1 唯一解、2 多解，有解时输出一组解。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef long double ld;

using namespace std;

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

/*
V < ld > x; int status = gauss(a, m, x);
V < ll > y; int mod_status = gauss_mod(b, m, p, y);
V < bitset < 3 > > c{3, 6};
V < int > z; int xor_status = gauss_xor(c, 2, z);
*/
