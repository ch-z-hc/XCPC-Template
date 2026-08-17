#include <bits/stdc++.h>
using namespace std;

// 自适应 Simpson 积分；函数在 [l, r] 上连续时使用，复杂度随函数变化而定。
template <class Function>
double adaptive_simpson(Function f, double l, double r, double eps = 1e-8,
                        int max_depth = 20) {
    auto simpson = [&](double a, double b) {
        double mid = (a + b) / 2;
        return (b - a) * (f(a) + 4 * f(mid) + f(b)) / 6;
    };
    auto solve = [&](auto &&self, double a, double b, double whole,
                     double error, int depth) -> double {
        double mid = (a + b) / 2;
        double left = simpson(a, mid), right = simpson(mid, b);
        if (depth == 0 || abs(left + right - whole) <= 15 * error)
            return left + right + (left + right - whole) / 15;
        return self(self, a, mid, left, error / 2, depth - 1) +
               self(self, mid, b, right, error / 2, depth - 1);
    };
    return solve(solve, l, r, simpson(l, r), eps, max_depth);
}
