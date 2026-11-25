#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
平面向量运算、方向判断和极角比较均为 O(1)，极角排序 O(n log n)。
相等判断用 EPS；排序直接比较坐标和叉积。取单位向量、指定起始射线时方向须非零。
*/

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
    P operator -(P p) const { return {x - p.x, y - p.y}; }
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

/*
从非零方向 dir 逆时针排序，同向向量默认最先，include 为 false 时最后。
零向量始终在所有方向之前，同一方向不按长度区分。
*/
auto polar_cmp(P dir, bool include = true) {
    auto part = [=](P p) -> int {
        if (p.x == 0 && p.y == 0) return -1;
        return include ? polar_cmpX(p, dir) : ! polar_cmpX(dir, p);
    };
    return [=](P a, P b) {
        int x = part(a), y = part(b);
        return x != y ? x < y : polar_cmpX(a, b);
    };
}

/*
P o{0, 0}, q{2, 0}, p{1, 1}; P u = q - o, v = p - o;
P mid = (p + q) / 2, twice = u * 2; cout << mid;
bool equal = p == q, before = p < q; int sx = sign(p.x), order = cmp(p.x, q.x);
db len = u.abs(), squared = u.abs2(), dist = p.distTo(q);
db dot = u.dot(v), det = u.det(v), angle = rad(u, v), arg = v.alpha();
db twice_area = cross(o, q, p); int side = crossop(o, q, p);
P unit = u.unit(), normal = u.rot90(), rotated = u.rot(angle);
int half = v.quadX(); bool earlier = polar_cmpX(u, v); // 从正 X 轴起的半区、极角序

// 从射线 o -> q 开始，绕 o 逆时针排序；与 o 重合的点始终最先。
V < P > ps{p, o, q};
auto by_angle = polar_cmp(q - o, true);      // 起始射线上的点最先，true 可省略
// auto by_angle = polar_cmp(q - o, false);  // 起始射线上的点最后
sort(ps.begin(), ps.end(), [&](P a, P b) { return by_angle(a - o, b - o); });
*/
