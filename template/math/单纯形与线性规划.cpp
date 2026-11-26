/*
最大化 c·x，约束 Ax <= b、x >= 0。矩阵 0-based，行列分别对应约束和变量。
eps 为绝对容差，系数量级悬殊时先缩放。换基 O(nm)，最坏指数时间。来源 Stanford / KACTL，MIT 许可。
status 为 0、1、2 分别表示无解、有最优解、无界。无界时 x 是一组可行解。
*/

#include <bits/stdc++.h>

using ld = long double;

struct Simplex {
    struct Result {
        int status;
        ld value;
        std::vector<ld> x;
    };
    int n = 0, m = 0;
    ld eps = 1e-12L;
    std::vector<int> nbas, bas;
    std::vector<std::vector<ld>> a;

    void pivot(int r, int s) {
        ld inv = 1 / a[r][s];
        for (int i = 0; i < m + 2; i ++)
            if (i != r)
                for (int j = 0; j < n + 2; j ++)
                    if (j != s) a[i][j] -= a[r][j] * a[i][s] * inv;
        for (int j = 0; j < n + 2; j ++)
            if (j != s) a[r][j] *= inv;
        for (int i = 0; i < m + 2; i ++)
            if (i != r) a[i][s] *= -inv;
        a[r][s] = inv;
        std::swap(bas[r], nbas[s]);
    }

    bool phase(int row) {
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; j ++) {
                if (row == m && nbas[j] == -1) continue;
                if (a[row][j] < -eps && (s == -1 || nbas[j] < nbas[s])) s = j;
            }
            if (s == -1) return true;
            int r = -1;
            for (int i = 0; i < m; i ++)
                if (a[i][s] > eps) {
                    if (r == -1) {
                        r = i;
                        continue;
                    }
                    ld x = a[i][n + 1] / a[i][s];
                    ld y = a[r][n + 1] / a[r][s];
                    if (x < y - eps || (std::abs(x - y) <= eps && bas[i] < bas[r])) r = i;
                }
            if (r == -1) return false;
            pivot(r, s);
        }
    }

    Result solve(const std::vector<std::vector<ld>> &A, const std::vector<ld> &b,
                 const std::vector<ld> &c) {
        m = int(b.size());
        n = int(c.size());
        a.assign(m + 2, std::vector<ld>(n + 2));
        bas.resize(m);
        nbas.resize(n + 1);
        for (int i = 0; i < m; i ++) {
            for (int j = 0; j < n; j ++) a[i][j] = A[i][j];
            bas[i] = n + i;
            a[i][n] = -1;
            a[i][n + 1] = b[i];
        }
        for (int j = 0; j < n; j ++) nbas[j] = j, a[m][j] = -c[j];
        nbas[n] = -1;
        a[m + 1][n] = 1;
        int r = -1;
        for (int i = 0; i < m; i ++)
            if (r == -1 || a[i][n + 1] < a[r][n + 1]) r = i;
        if (r != -1 && a[r][n + 1] < -eps) {
            pivot(r, n);
            if (!phase(m + 1) || std::abs(a[m + 1][n + 1]) > eps) return {0, 0, {}};
            for (int i = 0; i < m; i ++)
                if (bas[i] == -1) {
                    int s = -1;
                    for (int j = 0; j <= n; j ++)
                        if (std::abs(a[i][j]) > eps && (s == -1 || nbas[j] < nbas[s])) s = j;
                    if (s != -1) pivot(i, s);
                }
        }
        bool ok = phase(m);
        std::vector<ld> x(n);
        for (int i = 0; i < m; i ++)
            if (0 <= bas[i] && bas[i] < n) x[bas[i]] = a[i][n + 1];
        ld inf = std::numeric_limits<ld>::infinity();
        return {ok ? 1 : 2, ok ? a[m][n + 1] : inf, x};
    }
};

/*
Simplex lp; auto res = lp.solve({{1, 1}}, {4}, {2, 1});
if (res.status == 1) cout << res.value << ' ' << res.x[0];
*/
