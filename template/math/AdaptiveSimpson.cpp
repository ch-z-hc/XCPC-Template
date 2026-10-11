#include <bits/stdc++.h>

using namespace std;

// 自适应 Simpson 积分；函数在 [l, r] 上连续时使用，接口不涉及数组下标。
// 复杂度随被积函数和误差要求变化，递归深度不超过 max_dep。

template < class F >
double simpson(F f, double l, double r, double eps = 1e-8,
                        int max_dep = 20) {
    auto simpson = [&](double a, double b) {
        double mid = (a + b) / 2;
        return (b - a) * (f(a) + 4 * f(mid) + f(b)) / 6;
    };
    auto solve = [&](auto &&self, double a, double b, double s,
                     double err, int dep) -> double {
        double mid = (a + b) / 2;
        double x = simpson(a, mid), y = simpson(mid, b);
        if (dep == 0 || abs(x + y - s) <= 15 * err)
            return x + y + (x + y - s) / 15;
        return self(self, a, mid, x, err / 2, dep - 1) +
               self(self, mid, b, y, err / 2, dep - 1);
    };
    return solve(solve, l, r, simpson(l, r), eps, max_dep);
}

// 调用示例：
// double ans = simpson([](double x) { return x * x; }, 0, 1);
