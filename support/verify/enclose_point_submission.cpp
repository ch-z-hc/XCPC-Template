#include <bits/stdc++.h>

using namespace std;

#define V vector
typedef long long ll;

// QOJ 23596：包围点。独立解题示例，不修改几何模板。
// 坐标绝对值不超过 1e9，叉积使用 ll 精确计算。
// 预处理 O(n log n)，每次询问 O(log n)，空间 O(n)。
// 原点编号 1-based，凸包下标 0-based，顶点按逆时针排列。
struct P {
    ll x = 0, y = 0;
    int id = 0;

    bool operator < (P p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }

    P operator -(P p) { return {x - p.x, y - p.y}; }
    ll det(P p) { return x * p.y - y * p.x; }
};

ll cross(P a, P b, P c) { return (b - a).det(c - a); }

V < P > convexHull(V < P > p) {
    sort(p.begin(), p.end());
    int n = p.size(), k = 0;
    if (n <= 1) return p;
    V < P > h(n * 2);
    for (int i = 0; i < n; h[k ++] = p[i ++])
        while (k > 1 && cross(h[k - 2], h[k - 1], p[i]) <= 0)
            -- k;
    for (int i = n - 2, t = k; i >= 0; h[k ++] = p[i --])
        while (k > t && cross(h[k - 2], h[k - 1], p[i]) <= 0)
            -- k;
    h.resize(k - 1);
    return h;
}

// 只找严格包含 p 的三角形；候选点最多 5 个，直接枚举。
V < P > find_triangle(const V < P > & a, P p) {
    int n = a.size();
    for (int i = 0; i < n; ++ i)
        for (int j = i + 1; j < n; ++ j)
            for (int k = j + 1; k < n; ++ k) {
                ll x = cross(a[i], a[j], p);
                ll y = cross(a[j], a[k], p);
                ll z = cross(a[k], a[i], p);
                if ((x > 0 && y > 0 && z > 0) ||
                    (x < 0 && y < 0 && z < 0))
                    return {a[i], a[j], a[k]};
            }
    return {};
}

// 严格内部点：返回所属扇形左端点 l，满足 1 <= l <= h.size() - 2。
// 外部点、边界点和退化凸包：统一返回 -1。
int locate(const V < P > & h, P p) {
    int n = h.size();
    if (n < 3) return -1;
    if (cross(h[0], h[1], p) <= 0 ||
        cross(h[0], h[n - 1], p) >= 0) return -1;
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) / 2;
        if (cross(h[0], h[mid], p) >= 0) l = mid;
        else r = mid;
    }
    if (cross(h[l], h[r], p) <= 0) return -1;
    return l;
}

V < P > query(const V < P > & h, P extra, P p) {
    int l = locate(h, p), n = h.size();
    if (l == -1) return {};

    // 不在扇形分界线上：所属三角形已经是最优答案。
    if (cross(h[0], h[l], p) != 0)
        return {h[0], h[l], h[l + 1]};

    // 在内部对角线上：两侧扇形合成一个严格包含 p 的四边形。
    // locate 保证此时 2 <= l <= n - 2，不会访问 h[-1] 或 h[n]。
    V < P > quad = {h[0], h[l - 1], h[l], h[l + 1]};
    V < P > a = quad;
    if (n > 4) {
        // 取一个不属于 quad 的凸包顶点，它不在 quad 的两条对角线上。
        a.push_back(h[l == 2 ? n - 1 : 1]);
    } else if (extra.id) {
        a.push_back(extra);
    }

    V < P > ans = find_triangle(a, p);
    if (! ans.empty()) return ans;
    // 无三角形只可能发生在 n == 4、p 是对角线交点且 extra 不存在时。
    return quad;
}

void solve() {
    int n, q; cin >> n >> q;
    V < P > p(n);
    for (int i = 0; i < n; ++ i) {
        cin >> p[i].x >> p[i].y;
        p[i].id = i + 1;
    }

    V < P > h = convexHull(p);
    P extra;
    if (h.size() == 4) {
        // 寻找不在两条对角线所在直线上的原点，最多需要一个。
        for (P x : p)
            if (cross(h[0], h[2], x) != 0 &&
                cross(h[1], h[3], x) != 0) {
                extra = x;
                break;
            }
    }

    while (q --) {
        P x; cin >> x.x >> x.y;
        V < P > ans = query(h, extra, x);
        if (ans.empty()) {
            cout << -1 << '\n';
            continue;
        }
        cout << ans.size() << '\n';
        for (P x : ans) cout << x.id << ' ';
        cout << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int t; cin >> t;
    while (t --) solve();
    return 0;
}
