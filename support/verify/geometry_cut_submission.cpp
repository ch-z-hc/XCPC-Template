#include "../../template/geometry/点与向量.cpp"
#include "../../template/geometry/直线与线段.cpp"
#include "../../template/geometry/多边形.cpp"

void solve() {
    int n; cin >> n;
    V < P > p(n);
    for (int i = 0; i < n; ++ i) {
        cin >> p[i].x >> p[i].y;
    }
    int q; cin >> q;
    while (q --) {
        P p1, p2;
        cin >> p1.x >> p1.y >> p2.x >> p2.y;
        cout << fixed << setprecision(10) << area(convexCut(p, p1, p2)) << '\n';
    }
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    while (t --) {
        solve();
    }
    return 0;
}
