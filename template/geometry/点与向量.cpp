#include <bits/stdc++.h>

#define V vector

using namespace std;

#define db long double
// #define db long long

const db EPS = 1e-9;

#define cross(p1, p2, p3) (((p2) - (p1)).det((p3) - (p1)))
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
        if (x != p.x) return x < p.x;
        return y < p.y;
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

    int quadY() const { return x > 0 || (x == 0 && y >= 0); }
    int quadX() const { return y > 0 || (y == 0 && x >= 0); }

    P rot90() { return P(-y, x); }
    P rot(db alpha) {
        return { x * cos(alpha) - y * sin(alpha), x * sin(alpha) + y * cos(alpha)};
    }
    P unit() { return *this / abs(); }
};

ostream & operator << (ostream &out, const P &A) {
    out << A.x << " " << A.y;
    return out;
}

// 向量夹角与极角排序，O(1)；排序使用精确叉积，不用 EPS 比较角度。
db rad(P p1, P p2) {
    // 求向量 p1 p2 的角度（逆时针为正，顺时针为负，范围 (- pi, pi]）
    return atan2l(p1.det(p2), p1.dot(p2));
}

bool polar_cmpX(P a, P b) {
    // 极角排序 X 轴正半轴（包含）开始逆时针转
    bool za = a.x == 0 && a.y == 0, zb = b.x == 0 && b.y == 0;
    if (za || zb) return za && ! zb;
    if (a.quadX() != b.quadX())
        return a.quadX() > b.quadX();
    return a.det(b) > 0;
}

bool polar_cmpY(P a, P b) {
    // 极角排序 Y 轴负半轴（不包含）开始逆时针转
    bool za = a.x == 0 && a.y == 0, zb = b.x == 0 && b.y == 0;
    if (za || zb) return za && ! zb;
    if (a.quadY() != b.quadY())
        return a.quadY() > b.quadY();
    return a.det(b) > 0;
}
