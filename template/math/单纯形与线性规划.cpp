#include <bits/stdc++.h>

#define V vector

typedef long double ld;

using namespace std;

// 两阶段单纯形：最大化 c*x，约束 A*x <= b 且 x >= 0，矩阵 0-based。
// solve(A, b, c) 返回 {status, value, x}：0 无解 / 1 最优 / 2 无界。
// 输入尺寸须匹配，允许零个约束或变量；无解时 x 为空，无界时 x 为可行解。
// 单次换基 O(nm)，最坏指数时间，空间 O(nm)；不保证病态输入的数值稳定性。
// eps 为绝对容差，必要时先缩放；采用 Bland 规则减少退化循环风险。
// 参考 Stanford / KACTL Simplex，MIT；原许可见参考资料。

struct Simplex {
    struct Result { int status; ld value; V < ld > x; };
    int n = 0, m = 0;
    ld eps = 1e-12L;
    V < int > nonbasic, basic;
    V < V < ld > > a;

    void pivot(int r, int s) {
        ld inv = 1 / a[r][s];
        for (int i = 0; i < m + 2; ++ i) if (i != r)
            for (int j = 0; j < n + 2; ++ j) if (j != s)
                a[i][j] -= a[r][j] * a[i][s] * inv;
        for (int j = 0; j < n + 2; ++ j) if (j != s) a[r][j] *= inv;
        for (int i = 0; i < m + 2; ++ i) if (i != r) a[i][s] *= -inv;
        a[r][s] = inv;
        swap(basic[r], nonbasic[s]);
    }

    bool phase(int row) {
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; ++ j) {
                if (row == m && nonbasic[j] == -1) continue;
                if (a[row][j] < -eps &&
                    (s == -1 || nonbasic[j] < nonbasic[s])) s = j;
            }
            if (s == -1) return true;
            int r = -1;
            for (int i = 0; i < m; ++ i) if (a[i][s] > eps) {
                if (r == -1) { r = i; continue; }
                ld x = a[i][n + 1] / a[i][s];
                ld y = a[r][n + 1] / a[r][s];
                if (x < y - eps ||
                    (abs(x - y) <= eps && basic[i] < basic[r])) r = i;
            }
            if (r == -1) return false;
            pivot(r, s);
        }
    }

    Result solve(const V < V < ld > > &A, const V < ld > &b,
                 const V < ld > &c) {
        m = int(b.size()); n = int(c.size());
        a.assign(m + 2, V < ld > (n + 2));
        basic.resize(m); nonbasic.resize(n + 1);
        for (int i = 0; i < m; ++ i) {
            for (int j = 0; j < n; ++ j) a[i][j] = A[i][j];
            basic[i] = n + i; a[i][n] = -1; a[i][n + 1] = b[i];
        }
        for (int j = 0; j < n; ++ j) nonbasic[j] = j, a[m][j] = -c[j];
        nonbasic[n] = -1; a[m + 1][n] = 1;
        int r = -1;
        for (int i = 0; i < m; ++ i)
            if (r == -1 || a[i][n + 1] < a[r][n + 1]) r = i;
        if (r != -1 && a[r][n + 1] < -eps) {
            pivot(r, n);
            if (! phase(m + 1) || abs(a[m + 1][n + 1]) > eps)
                return {0, 0, {}};
            for (int i = 0; i < m; ++ i) if (basic[i] == -1) {
                int s = -1;
                for (int j = 0; j <= n; ++ j)
                    if (abs(a[i][j]) > eps &&
                        (s == -1 || nonbasic[j] < nonbasic[s])) s = j;
                if (s != -1) pivot(i, s);
            }
        }
        bool ok = phase(m);
        V < ld > x(n);
        for (int i = 0; i < m; ++ i)
            if (0 <= basic[i] && basic[i] < n) x[basic[i]] = a[i][n + 1];
        ld inf = numeric_limits < ld >::infinity();
        return {ok ? 1 : 2, ok ? a[m][n + 1] : inf, x};
    }
};

// 用法：Simplex lp; auto ans = lp.solve(A, b, c);
// if (ans.status == 1) { ld optimum = ans.value; }
