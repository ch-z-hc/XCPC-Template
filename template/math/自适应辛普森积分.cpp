/*
平滑函数定积分，eps > 0 为绝对误差目标，尖峰和高频振荡须先分段。
max_dep >= 0，最坏 O(2^max_dep)。达到深度上限可能超出目标误差，采样及结果须能用 double 表示。
*/

#include <bits/stdc++.h>

typedef long double ld;

using namespace std;

template < class F >
double simpson(F f, double l, double r, double eps = 1e-8, int max_dep = 20) {
    auto simpson = [&](double a, double b) {
        double mid = (ld(a) + b) / 2;
        return (ld(b) - a) * (f(a) + 4.L * f(mid) + f(b)) / 6;
    };
    auto solve = [&](auto &&self, double a, double b, ld s,
                     ld err, int dep) -> ld {
        double mid = (ld(a) + b) / 2;
        ld x = simpson(a, mid), y = simpson(mid, b);
        if (dep == 0)
            return x + y + (x + y - s) / 15;
        if (abs(x + y - s) <= 15 * err) {
            double lm = (ld(a) + mid) / 2;
            double rm = (ld(mid) + b) / 2;
            ld u = simpson(a, lm) + simpson(lm, mid);
            ld v = simpson(mid, rm) + simpson(rm, b);
            if (abs(u - x) + abs(v - y) <= 15 * err)
                return u + v + (u + v - x - y) / 15;
        }
        return self(self, a, mid, x, err / 2, dep - 1) +
               self(self, mid, b, y, err / 2, dep - 1);
    };
    return solve(solve, l, r, simpson(l, r), eps, max_dep);
}

// double ans = simpson([](double x) { return x * x; }, 0, 1);
