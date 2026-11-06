#include <bits/stdc++.h>

typedef long double ld;

using namespace std;

// 连续单峰函数三分，返回极值位置的近似值。
// l <= r；极值区间外须严格单调，非最优平台会使三分误判。
// 调用函数次数 O(times)，额外空间 O(1)。

template < class F >
ld ternary_min(F f, ld l, ld r, int times = 200) {
    for (int i = 0; i < times; ++ i) {
        ld d = r / 3 - l / 3, a = l + d, b = r - d;
        if (f(a) <= f(b)) r = b; else l = a;
    }
    return l + (r / 2 - l / 2);
}

template < class F >
ld ternary_max(F f, ld l, ld r, int times = 200) {
    for (int i = 0; i < times; ++ i) {
        ld d = r / 3 - l / 3, a = l + d, b = r - d;
        if (f(a) >= f(b)) r = b; else l = a;
    }
    return l + (r / 2 - l / 2);
}

// 用法：ld x = ternary_min([](ld x) { return x * x; }, -10, 10);
