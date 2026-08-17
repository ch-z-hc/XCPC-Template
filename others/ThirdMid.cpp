#include <bits/stdc++.h>
using namespace std;

// 单峰函数三分；返回极值位置近似值。
template <class Function>
long double ternary_min(Function f, long double left, long double right,
                        int iterations = 200) {
    for (int i = 0; i < iterations; ++i) {
        long double a = (2 * left + right) / 3, b = (left + 2 * right) / 3;
        if (f(a) <= f(b)) right = b; else left = a;
    }
    return (left + right) / 2;
}

template <class Function>
long double ternary_max(Function f, long double left, long double right,
                        int iterations = 200) {
    for (int i = 0; i < iterations; ++i) {
        long double a = (2 * left + right) / 3, b = (left + 2 * right) / 3;
        if (f(a) >= f(b)) right = b; else left = a;
    }
    return (left + right) / 2;
}
