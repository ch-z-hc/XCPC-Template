#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 先复制 Point.cpp、Line.cpp、Circle.cpp。
// 随机增量最小圆覆盖，期望 O(n)，最坏 O(n^3)，额外空间 O(n)。
// 空点集返回原点半径 0 的圆；允许重复点和共线点。
Circle min_circle(V < P > a) {
    mt19937 rng(static_cast<unsigned> (chrono::steady_clock::now().time_since_epoch().count()));
    shuffle(a.begin(), a.end(), rng);
    Circle ans{{0, 0}, 0};
    for (int i = 0; i < int(a.size()); ++ i) if (! ans.contains(a[i])) {
        ans = {a[i], 0};
        for (int j = 0; j < i; ++ j) if (! ans.contains(a[j])) {
            ans = diameter_circle(a[i], a[j]);
            for (int k = 0; k < j; ++ k) if (! ans.contains(a[k])) {
                auto c = circumcircle(a[i], a[j], a[k]);
                if (c) ans = *c;
                else {
                    ans = diameter_circle(a[i], a[j]);
                    for (auto [u, v] : {pair < P, P >{a[i], a[k]}, {a[j], a[k]}}) {
                        Circle d = diameter_circle(u, v);
                        if (d.r > ans.r) ans = d;
                    }
                }
            }
        }
    }
    return ans;
}
