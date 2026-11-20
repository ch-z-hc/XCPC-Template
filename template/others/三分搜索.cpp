#include <bits/stdc++.h>

typedef long double ld;

using namespace std;

// 在 [l, r] 内找单峰函数的极值位置，要求 l <= r。极值两侧须严格单调，中途出现平台可能舍掉答案。
// 返回近似位置，函数值需再算一次。迭代 times 轮，额外空间 O(1)。

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

// ld x = ternary_min([](ld x) { return x * x; }, -10, 10);
