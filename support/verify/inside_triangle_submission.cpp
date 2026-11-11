#include "../../template/geometry/点与向量.cpp"
#include "../../template/geometry/嵌套凸多边形切线.cpp"

typedef long long ll;

// QOJ 14714：三角形非严格包含内凸多边形。
// 几何预处理 O(n + m)，前缀和与单调指针计数 O(n)，答案用 ll。
ll count_triangles(const V < P > & p, const V < P > & q) {
    NestedHull hull; hull.build(p, q);
    int n = p.size(), j = 1;
    V < ll > sum(2 * n + 1);
    for (int i = 0; i < 2 * n; ++ i) sum[i + 1] = sum[i] + hull.r[i];
    ll ans = 0;
    for (int i = 0; i < n; ++ i) {
        j = max(j, i + 1);
        while (j <= hull.r[i] && hull.r[j] < hull.l[i]) ++ j;
        if (j <= hull.r[i])
            ans += sum[hull.r[i] + 1] - sum[j] -
                   1LL * (hull.r[i] - j + 1) * (hull.l[i] - 1);
    }
    // 每个三角形分别以三个顶点为起点计入。
    return ans / 3;
}

#ifndef INSIDE_TRIANGLE_NO_MAIN
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int t; cin >> t;
    while (t --) {
        int n; cin >> n;
        V < P > p(n);
        for (auto & x : p) cin >> x.x >> x.y;
        int m; cin >> m;
        V < P > q(m);
        for (auto & x : q) cin >> x.x >> x.y;
        cout << count_triangles(p, q) << '\n';
    }
}
#endif
