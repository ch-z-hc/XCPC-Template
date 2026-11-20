#include <bits/stdc++.h>

#define V vector

typedef long double ld;

using namespace std;

// 求 max c·x，约束为 Ax <= b、x >= 0。矩阵从 0 编号，行数与 b 相同，列数与 c 相同。
// status 为 0 表示无解，1 表示找到最优解，2 表示无界。无界时 x 仍是一组可行解。
// eps 是绝对容差，系数量级相差大时先缩放。每次换基 O(nm)，最坏指数时间，空间 O(nm + n + m)。
// 来源 Stanford / KACTL Simplex，MIT 许可见参考资料。

struct Simplex {
    struct Result {
        int status;
        ld value;
        V < ld > x;
    };
    int n = 0, m = 0;
    ld eps = 1e-12L;
    V < int > nbas, bas;
    V < V < ld > > a;

    void pivot(int r, int s) {
        ld inv = 1 / a[r][s];
        for (int i = 0; i < m + 2; ++ i) if (i != r)
            for (int j = 0; j < n + 2; ++ j) if (j != s)
                a[i][j] -= a[r][j] * a[i][s] * inv;
        for (int j = 0; j < n + 2; ++ j) if (j != s) a[r][j] *= inv;
        for (int i = 0; i < m + 2; ++ i) if (i != r) a[i][s] *= -inv;
        a[r][s] = inv;
        swap(bas[r], nbas[s]);
    }

    bool phase(int row) {
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; ++ j) {
                if (row == m && nbas[j] == -1) continue;
                if (a[row][j] < -eps &&
                    (s == -1 || nbas[j] < nbas[s])) s = j;
            }
            if (s == -1) return true;
            int r = -1;
            for (int i = 0; i < m; ++ i) if (a[i][s] > eps) {
                if (r == -1) { r = i; continue; }
                ld x = a[i][n + 1] / a[i][s];
                ld y = a[r][n + 1] / a[r][s];
                if (x < y - eps ||
                    (abs(x - y) <= eps && bas[i] < bas[r])) r = i;
            }
            if (r == -1) return false;
            pivot(r, s);
        }
    }

    Result solve(const V < V < ld > > &A, const V < ld > &b,
                 const V < ld > &c) {
        m = int(b.size()); n = int(c.size());
        a.assign(m + 2, V < ld > (n + 2));
        bas.resize(m); nbas.resize(n + 1);
        for (int i = 0; i < m; ++ i) {
            for (int j = 0; j < n; ++ j) a[i][j] = A[i][j];
            bas[i] = n + i;
            a[i][n] = -1;
            a[i][n + 1] = b[i];
        }
        for (int j = 0; j < n; ++ j) nbas[j] = j, a[m][j] = -c[j];
        nbas[n] = -1; a[m + 1][n] = 1;
        int r = -1;
        for (int i = 0; i < m; ++ i)
            if (r == -1 || a[i][n + 1] < a[r][n + 1]) r = i;
        if (r != -1 && a[r][n + 1] < -eps) {
            pivot(r, n);
            if (! phase(m + 1) || abs(a[m + 1][n + 1]) > eps)
                return {0, 0, {}};
            for (int i = 0; i < m; ++ i) if (bas[i] == -1) {
                int s = -1;
                for (int j = 0; j <= n; ++ j)
                    if (abs(a[i][j]) > eps &&
                        (s == -1 || nbas[j] < nbas[s])) s = j;
                if (s != -1) pivot(i, s);
            }
        }
        bool ok = phase(m);
        V < ld > x(n);
        for (int i = 0; i < m; ++ i)
            if (0 <= bas[i] && bas[i] < n) x[bas[i]] = a[i][n + 1];
        ld inf = numeric_limits < ld >::infinity();
        return {ok ? 1 : 2, ok ? a[m][n + 1] : inf, x};
    }
};

// Simplex lp; auto ans = lp.solve(A, b, c);
// if (ans.status == 1) { ld optimum = ans.value; }
