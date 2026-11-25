#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

/*
先复制「点与向量」「直线与线段」「圆」。最小覆盖圆期望 O(n)，最坏 O(n³)。
允许重复点和共线点，空集返回圆心在原点、半径为 0 的圆。
*/

Circle min_circle(V < P > a) {
    mt19937 rng(unsigned(chrono::steady_clock::now().time_since_epoch().count()));
    shuffle(a.begin(), a.end(), rng);
    Circle ans{{0, 0}, 0};
    for (int i = 0; i < int(a.size()); ++ i) if (! ans.contains(a[i])) {
        ans = {a[i], 0};
        for (int j = 0; j < i; ++ j) if (! ans.contains(a[j])) {
            ans = diameter_circle(a[i], a[j]);
            for (int k = 0; k < j; ++ k) if (! ans.contains(a[k])) {
                if (! circumcircle(a[i], a[j], a[k], ans)) {
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

// Circle c = min_circle({{0, 0}, {2, 0}, {1, 1}}); // c.o 为圆心，c.r 为半径
