#include <bits/stdc++.h>

typedef long double ld;

using namespace std;

// 自适应 Simpson 积分；用于局部采样能解析的平滑函数，无数组下标或模数。
// eps > 0，max_dep >= 0；eps 是绝对误差估计，不是任意连续函数的严格保证。
// 尖峰或高频振荡需按已知位置分段；达到深度上限仍返回当前近似值。
// 采样值与积分近似须可用 double 表示；使用范围更宽的 long double 中间量。
// 复杂度随函数与误差要求变化，最坏 O(2^max_dep)，递归空间 O(max_dep)。

template < class F >
double simpson(F f, double l, double r, double eps = 1e-8,
                        int max_dep = 20) {
    auto simpson = [&](double a, double b) {
        double mid = (ld(a) + b) / 2;
        return double((ld(b) - a) *
                      (f(a) + 4.L * f(mid) + f(b)) / 6);
    };
    auto solve = [&](auto &&self, double a, double b, double s,
                     double err, int dep) -> double {
        double mid = (ld(a) + b) / 2;
        double x = simpson(a, mid), y = simpson(mid, b);
        if (dep == 0)
            return x + y + (x + y - s) / 15;
        if (abs(x + y - s) <= 15 * err) {
            double lm = (ld(a) + mid) / 2;
            double rm = (ld(mid) + b) / 2;
            double u = simpson(a, lm) + simpson(lm, mid);
            double v = simpson(mid, rm) + simpson(rm, b);
            // 分别检查两侧误差，避免左右抵消导致提前判停。
            if (abs(u - x) + abs(v - y) <= 15 * err)
                return u + v + (u + v - x - y) / 15;
        }
        return self(self, a, mid, x, err / 2, dep - 1) +
               self(self, mid, b, y, err / 2, dep - 1);
    };
    return solve(solve, l, r, simpson(l, r), eps, max_dep);
}

// 调用示例：
// double ans = simpson([](double x) { return x * x; }, 0, 1);
