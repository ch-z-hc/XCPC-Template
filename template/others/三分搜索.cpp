/*
在 [l, r] 上找单峰函数的极值位置，极值两侧须严格单调，中途不能有平台。
迭代 times 轮，耗时 O(times) 次函数求值，返回近似位置。
*/

#include <bits/stdc++.h>

typedef long double ld;

using namespace std;

template < class F > ld ternary_min(F f, ld l, ld r, int times = 200) {
    for (int i = 0; i < times; i ++) {
        ld d = r / 3 - l / 3, a = l + d, b = r - d;
        if (f(a) <= f(b))
            r = b;
        else
            l = a;
    }
    return l + (r / 2 - l / 2);
}

template < class F > ld ternary_max(F f, ld l, ld r, int times = 200) {
    for (int i = 0; i < times; i ++) {
        ld d = r / 3 - l / 3, a = l + d, b = r - d;
        if (f(a) >= f(b))
            r = b;
        else
            l = a;
    }
    return l + (r / 2 - l / 2);
}

/*
ld lo = ternary_min([](ld x) { return x * x; }, -10, 10);
ld hi = ternary_max([](ld x) { return -x * x; }, -10, 10);
*/
