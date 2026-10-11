#include <bits/stdc++.h>

typedef long double ld;

using namespace std;

// 连续单峰函数三分；接口不涉及数组下标，返回极值位置近似值。
// 调用函数次数 O(times)，额外空间 O(1)。

template < class F >
ld ternary_min(F f, ld l, ld r, int times = 200) {
    for (int i = 0; i < times; ++ i) {
        ld a = (2 * l + r) / 3, b = (l + 2 * r) / 3;
        if (f(a) <= f(b)) r = b; else l = a;
    }
    return (l + r) / 2;
}

template < class F >
ld ternary_max(F f, ld l, ld r, int times = 200) {
    for (int i = 0; i < times; ++ i) {
        ld a = (2 * l + r) / 3, b = (l + 2 * r) / 3;
        if (f(a) >= f(b)) r = b; else l = a;
    }
    return (l + r) / 2;
}

// 调用示例：
// ld x = ternary_min([](ld x) { return x * x; }, -10, 10);
