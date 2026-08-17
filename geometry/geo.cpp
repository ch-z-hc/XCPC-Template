#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << ' '

using namespace std;

#define db long double
// #define db long long

const db EPS = 1e-9;

#define cross(p1, p2, p3) (p2 - p1).det(p3 - p1)
#define crossop(p1, p2, p3) sign(cross(p1, p2, p3))

int sign(db a) { return a < -EPS ? -1 : a > EPS; }
int cmp(db a, db b) { return sign(a - b); }

struct P {
    db x, y;
    P() {}
    P(db _x, db _y) : x(_x), y(_y) {}

    P operator +(P p) { return {x + p.x, y + p.y}; }
    P operator -(P p) { return {x - p.x, y - p.y}; }
    P operator *(db d) { return {x * d, y * d}; }
    P operator /(db d) { return {x / d, y / d}; }

    bool operator < (P p) const {
        int c = cmp(x, p.x);
        if (c) return c == -1;
        return cmp(y, p.y) == -1;
    }

    bool operator == (P p) const {
        return ! cmp(x, p.x) && ! cmp(y, p.y);
    }

    db abs2() { return x * x + y * y; }
    db abs() { return sqrt(x * x + y * y); }
    db distTo(P p) { return (*this - p).abs(); }
    db alpha() { return atan2(y, x); }
    db dot(P p) { return x * p.x + y * p.y; }
    db det(P p) { return x * p.y - y * p.x; }

    int quadY() const { return sign(x) == 1 || (sign(x) == 0 && sign(y) >= 0); }
    int quadX() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }

    P rot90() { return P(-y, x); }
    P rot(db alpha) { return { x * cos(alpha) - y * sin(alpha), x * sin(alpha) + y * cos(alpha)}; }
    P unit() { return *this / abs(); }
};

ostream & operator << (ostream &out, const P &A) {
    out << A.x << " " << A.y;
    return out;
}

// ------------------------------------
// 线段 / 线段

bool checkPll(P p1, P p2, P q1, P q2) {
    // 判断平行
    db a1 = cross(q1, q2, p1), a2 = - cross(q1, q2, p2);
    return sign(a1 + a2) == 0;
}

P isLL(P p1, P p2, P q1, P q2) {
    // 返回交点
    db a1 = cross(q1, q2, p1), a2 = - cross(q1, q2, p2);
    return (p1 * a2 + p2 * a1) / (a1 + a2);
}

bool intersect(db l1, db r1, db l2, db r2) {
    if (l1 > r1) swap(l1, r1);
    if (l2 > r2) swap(l2, r2);
    return cmp(r1, l2) >= 0 && cmp(r2, l1) >= 0;
}

// 严格相交：不在边界交叉
// 不严格相交：可以在边界交叉

bool isSS(P p1, P p2, P q1, P q2) {
    // 线段是否 不严格 相交
    // 1 : SS
    // 0 : unSS
    return intersect(p1.x, p2.x, q1.x, q2.x) &&
           intersect(p1.y, p2.y, q1.y, q2.y) && 
           crossop(p1, p2, q1) * crossop(p1, p2, q2) <= 0 && 
           crossop(q1, q2, p1) * crossop(q1, q2, p2) <= 0;
}

bool isSS_strict(P p1, P p2, P q1, P q2) {
    // 线段是否 严格 相交
    // 1 : SS
    // 0 : unSS
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) < 0 && 
           crossop(q1, q2, p1) * crossop(q1, q2, p2) < 0;
}

bool isLS(P p1, P p2, P q1, P q2) {
    // 直线 p1p2 是否 和线段 q1q2 不严格 相交
    // 1 : LS
    // 0 : unLS
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) <= 0;
}

bool isLS_strict(P p1, P p2, P q1, P q2) {
    // 直线 p1p2 是否 严格 穿过线段 q1q2
    // 1 : LS
    // 0 : unLS
    return crossop(p1, p2, q1) * crossop(p1, p2, q2) < 0;
}


bool isMiddle(db a, db m, db b) {
    return sign(a - m) == 0 || sign(b - m) == 0 || ((a < m) != (b < m));
}

bool isMiddle(P a, P m, P b) {
    return isMiddle(a.x, m.x, b.x) && isMiddle(a.y, m.y, b.y);
}

bool onSeg(P p1, P p2, P q) {
    // 是否在线段上 不严格
    return crossop(p1, p2, q) == 0 && isMiddle(p1, q, p2);
}

bool onSeg_strict(P p1, P p2, P q) {
    // 是否在线段上 严格
    return crossop(p1, p2, q) == 0 && 
           sign((q - p1).dot(p1 - p2)) * 
           sign((q - p2).dot(p1 - p2)) < 0;
}

P proj(P p1, P p2, P q) {
    // 求 q 到 p1 p2 的投影
    P dir = p2 - p1;
    return p1 + (p2 - p1) * (dir.dot(q - p1) / (dir).abs2());
}

P reflect(P p1, P p2, P q) {
    // 求 q 到 p1 p2 的反射点
    return proj(p1, p2, q) * 2 - q;
}

db nearest(P p1, P p2, P q) {
    // 求 q 到 p1 p2（线段）的最近点
    if (p1 == p2) return p1.distTo(q);
    P h = proj(p1, p2, q);
    if (isMiddle(p1, h, p2))
        return q.distTo(h);
    return min(p1.distTo(q), p2.distTo(q));
}

db disSS(P p1, P p2, P q1, P q2) {
    // 求线段 p1p2 和 线段 q1q2 的距离
    if (isSS(p1, p2, q1, q2)) return 0;
    db val1 = min(nearest(p1, p2, q1), nearest(p1, p2, q2));
    db val2 = min(nearest(q1, q2, p1), nearest(q1, q2, p2));
    return min(val1, val2);
}

db rad(P p1, P p2) {
    // 求向量 p1 p2 的角度（逆时针为正，顺时针为负，范围 (- pi, pi]）
    return atan2l(p1.det(p2), p1.dot(p2));
}

// ---------------------------------------------
// 多边形

db area(vector < P > ps) {
    // 求多边形面积（逆时针顺序给出点 -> 面积为正）
    db ret = 0; if (ps.size() == 0) return 0;
    for (int i = 0; i < (int)ps.size(); ++ i) {
        ret += ps[i].det(ps[(i + 1) % ps.size()]);
    }
    return ret / 2;
}

int contain(vector < P > ps, P p) {
    // 0 : outside
    // 1 : on_seg
    // 2 : inside

    // ps 是多边形，判断 p 在多边形 内部 / 线段上 / 外部
    int n = ps.size(), ret = 0;
    for (int i = 0; i < n; ++ i) {
        P u = ps[i], v = ps[(i + 1) % n];
        if (onSeg(u, v, p)) return 1;
        if (cmp(u.y, v.y) <= 0) swap(u, v);
        if (cmp(p.y, u.y) > 0 || cmp(p.y, v.y) <= 0)
            continue;
        ret ^= crossop(p, u, v) > 0;
    }
    return ret * 2;
}

vector < P > convexHull(vector < P > ps) {
    // 求凸包
    int n = ps.size();
    if (n <= 1) return ps;
    sort(ps.begin(), ps.end());
    vector < P > qs(n * 2);
    int k = 0;
    for (int i = 0; i < n; qs[k ++] = ps[i ++])
        while (k > 1 && crossop(qs[k - 2], qs[k - 1], ps[i]) <= 0)
            -- k;
    for (int i = n - 2, t = k; i >= 0; qs[k ++] = ps[i --])
        while (k > t && crossop(qs[k - 2], qs[k - 1], ps[i]) <= 0)
            -- k;
    qs.resize(k - 1);
    return qs;
}


vector < P > convexHullNonStrict(vector < P > ps) {
    // 求凸包（边界上的点也在凸包内）
    sort(ps.begin(), ps.end());
    ps.erase(unique(ps.begin(), ps.end()), ps.end());
    int n = ps.size();
    if (n <= 1) return ps;
    vector < P > qs(n * 2);
    int k = 0;
    for (int i = 0; i < n; qs[k ++] = ps[i ++])
        while (k > 1 && crossop(qs[k - 2], qs[k - 1], ps[i]) < 0)
            -- k;
    for (int i = n - 2, t = k; i >= 0; qs[k ++] = ps[i --])
        while (k > t && crossop(qs[k - 2], qs[k - 1], ps[i]) < 0)
            -- k;
    qs.resize(k - 1);
    return qs;
}

bool checkConvexHull(vector < P > p) {
    // 判断 p 多边形是否是凸包
    // 注意：不可通过再跑一次凸包判断点数是否相同
    // 因为点数相同 不能代表是凸包的连接方式
    int n = p.size();
    for (int i = 0; i < n; ++ i) {
        int pre = i - 1;
        int suf = i + 1;
        if (pre == -1) pre = n - 1;
        if (suf == n) suf = 0;
        if (crossop(p[i], p[suf], p[pre]) < 0)
            return 0;
    }
    return 1;
}

db convexDiameter(vector < P > ps) {
    // 旋转卡壳求 凸包直径
    int n = ps.size();
    if (n <= 1) return 0;
    int is = 0, js = 0;
    for (int k = 1; k < n; ++ k)
        is = ps[k] < ps[is] ? k : is,
        js = ps[js] < ps[k] ? k : js;
    int i = is, j = js;
    db ret = ps[i].distTo(ps[j]);
    do {
        if ((ps[(i + 1) % n] - ps[i]).det(ps[(j + 1) % n] - ps[j]) >= 0)
            (++ j) %= n;
        else (++ i) %= n;
        ret = max(ret, ps[i].distTo(ps[j]));
    } while (i != is || j != js);
    return ret;
}


vector < P > convexCut(vector<P> ps, P q1, P q2) {
    // 直线切割凸包，返回直线左边凸包的点
    vector < P > qs;
    int n = ps.size();
    for (int i = 0; i < n; ++ i) {
        P p1 = ps[i], p2 = ps[(i + 1) % n];
        int d1 = crossop(q1, q2, p1), d2 = crossop(q1, q2, p2);
        if (d1 >= 0)
            qs.push_back(p1);
        if (d1 * d2 < 0)
            qs.push_back(isLL(p1, p2, q1, q2));
    }
    return qs;
}

bool polar_cmpX(P a, P b) {
    // 极角排序 X 轴正半轴（包含）开始逆时针转
    if (a.quadX() != b.quadX()) 
        return a.quadX() > b.quadX();
    return sign(a.det(b)) > 0;
}

bool polar_cmpY(P a, P b) {
    // 极角排序 Y 轴负半轴（不包含）开始逆时针转
    if (a.quadY() != b.quadY()) 
        return a.quadY() > b.quadY();
    return sign(a.det(b)) > 0;
}

vector < P > MincowskySum(vector < P > p, vector < P > q) {
    // p, q 为两个凸包（顺序同方向），求这两个凸包的 闵可夫斯基和
    // 得保证 p, q 的第一个点 是排序后的
    // 即 尽可能在 convexHull 后再传入 p, q 
    if (p.size() == 0) return q;
    if (q.size() == 0) return p;
    vector < P > qs; qs.push_back(p[0] + q[0]);
    P p0 = p[0], q0 = q[0];
    for (int i = 0; i + 1 < (int)p.size(); ++ i)
        p[i] = p[i + 1] - p[i];
    for (int i = 0; i + 1 < (int)q.size(); ++ i)
        q[i] = q[i + 1] - q[i];
    p[p.size() - 1] = p0 - p[p.size() - 1];
    q[q.size() - 1] = q0 - q[q.size() - 1];
    qs.resize(p.size() + q.size() + 1);
    merge(p.begin(), p.end(), q.begin(), q.end(), qs.begin() + 1, polar_cmpY);
    for (int i = 1; i < (int)qs.size(); ++ i)
        qs[i] = qs[i] + qs[i - 1];
    qs.pop_back();
    return qs;
}

void solve() {
    int n; cin >> n;
    vector < P > p(n);
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
