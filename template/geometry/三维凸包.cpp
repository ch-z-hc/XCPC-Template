#include <bits/stdc++.h>

#define V vector
#define db long double

using namespace std;

// 独立三维片段；p 与外向三角面 faces 使用原输入的 0-based 编号。
// build(ps, eps, integer, seed) 可重复调用；默认保留浮点接口。
// 随机增量 + 冲突图，期望 O(n log n)；并非仅打乱后扫描所有面。
// 成品凸包空间 O(n)，工作冲突表最坏 O(n²)，不承诺最坏线性内存。
// dim 为 -1 / 0 / 1 / 2 / 3：空 / 点 / 线 / 面 / 体。
// dim < 3 时 faces 为空，area / volume 为 0，不计算平面凸包面积。
// 支持重复点与共面面片，不扰动输入；eps 是距离容差，默认 1e-9。
// 浮点模式只适合有限坐标、尺度适中、非零距离明显大于容差与舍入误差的数据。
// integer = 1 使用 __int128 精确判向，忽略 eps；P3 仍保存原输入。
// 浮点过滤仅在符号超过舍入误差界时返回，否则精确计算；不要用 -ffast-math。
// 此模式要求 long double 至少 64 位有效二进制位，坐标为 |值| <= 1e18 的整数，
// 三轴坐标跨度 Rx、Ry、Rz 满足 Rx * Ry * Rz <= 1e35，避免整数溢出。
// 面积、体积仍是浮点近似；整数缓存 ip 不供调用方修改。
// 若退化边界触发 fallback，改用 O(n² log n) 的朴素算法，快速界不再适用。
// 期望界针对随机排列；seed 可传 random_device{}()，默认固定种子便于复现。

struct P3 {
    db x = 0, y = 0, z = 0;
    P3 operator +(P3 p) const { return {x + p.x, y + p.y, z + p.z}; }
    P3 operator -(P3 p) const { return {x - p.x, y - p.y, z - p.z}; }
    P3 operator *(db d) const { return {x * d, y * d, z * d}; }
    P3 operator /(db d) const { return {x / d, y / d, z / d}; }
    db dot(P3 p) const { return x * p.x + y * p.y + z * p.z; }
    P3 det(P3 p) const {
        return {y * p.z - z * p.y, z * p.x - x * p.z,
                x * p.y - y * p.x};
    }
    db abs() const { return sqrtl(dot(*this)); }
    db at(int k) const { return k == 0 ? x : k == 1 ? y : z; }
};

struct Hull3D {
    typedef __int128_t Int;
    struct Face { int a, b, c; };
    struct Tri {
        array < int, 3 > v, e;
        V < int > pts;
        bool dead = 0;
    };
    V < P3 > p;
    V < array < long long, 3 > > ip;
    V < Face > faces;
    P3 inside;
    int dim = -1;
    db eps = 1e-9;
    bool integer = 0, fallback = 0;

    P3 normal(Face f) const {
        return (p[f.b] - p[f.a]).det(p[f.c] - p[f.a]);
    }

    array < Int, 3 > normal_int(int a, int b, int c) const {
        array < long long, 3 > u, v;
        for (int k = 0; k < 3; ++ k) {
            u[k] = ip[b][k] - ip[a][k];
            v[k] = ip[c][k] - ip[a][k];
        }
        return {(Int)u[1] * v[2] - (Int)u[2] * v[1],
                (Int)u[2] * v[0] - (Int)u[0] * v[2],
                (Int)u[0] * v[1] - (Int)u[1] * v[0]};
    }

    struct Plane {
        array < Int, 3 > w{};
        P3 v;
        array < double, 3 > approx{};
        int a;
        db eps;
    };
    Plane plane(int a, int b, int c) const {
        Plane f; f.a = a;
        if (integer) {
            f.w = normal_int(a, b, c);
            for (int k = 0; k < 3; ++ k) f.approx[k] = (double)f.w[k];
        } else f.v = normal({a, b, c}), f.eps = eps * f.v.abs();
        return f;
    }
    int side(const Plane &f, int d) const {
        if (integer) {
            array < long long, 3 > delta;
            double val = 0, error = 0;
            for (int k = 0; k < 3; ++ k) {
                delta[k] = ip[d][k] - ip[f.a][k];
                double t = f.approx[k] * (double)delta[k];
                val += t; error += fabs(t);
            }
            error *= 64 * numeric_limits < double >::epsilon();
            if (val > error) return 1;
            if (val < -error) return -1;
            Int v = 0;
            for (int k = 0; k < 3; ++ k) v += f.w[k] * delta[k];
            return (v > 0) - (v < 0);
        }
        db v = f.v.dot(p[d] - p[f.a]);
        return (v > f.eps) - (v < -f.eps);
    }
    int side(int a, int b, int c, int d) const { return side(plane(a, b, c), d); }

    bool line(int a, int b, int c) const {
        if (integer) {
            auto w = normal_int(a, b, c);
            return ! w[0] && ! w[1] && ! w[2];
        }
        return normal({a, b, c}).abs() <= eps * (p[b] - p[a]).abs();
    }

    // 共线边界可能产生零面积面；保留正确的朴素增量回退。
    void slow(V < int > &id) {
        fallback = 1; faces.clear();
        auto add = [&](int a, int b, int c, int d) {
            if (side(a, b, c, d) > 0) swap(b, c);
            faces.push_back({a, b, c});
        };
        add(id[0], id[1], id[2], id[3]); add(id[0], id[1], id[3], id[2]);
        add(id[0], id[2], id[3], id[1]); add(id[1], id[2], id[3], id[0]);
        for (int t = 4; t < (int)id.size(); ++ t) {
            int i = id[t]; set < pair < int, int > > edge;
            V < Face > keep;
            for (Face f : faces) {
                if (side(f.a, f.b, f.c, i) <= 0) {
                    keep.push_back(f); continue;
                }
                for (auto [a, b] : {pair{f.a, f.b}, pair{f.b, f.c}, pair{f.c, f.a}}) {
                    auto it = edge.find({b, a});
                    if (it == edge.end()) edge.insert({a, b});
                    else edge.erase(it);
                }
            }
            faces.swap(keep);
            for (auto [a, b] : edge) faces.push_back({a, b, i});
        }
    }

    void build(const V < P3 > &ps, db tolerance = 1e-9, bool exact = 0,
               unsigned seed = 712367821) {
        p = ps; faces.clear(); eps = tolerance; integer = exact;
        dim = -1; fallback = 0;
        int n = p.size(); ip.clear();
        if (integer) {
            ip.resize(n);
            for (int i = 0; i < n; ++ i) for (int k = 0; k < 3; ++ k)
                ip[i][k] = (long long)p[i].at(k);
        }
        if (! n) return;
        V < int > id(n); iota(id.begin(), id.end(), 0);
        if (n > 4) { mt19937 rng(seed); shuffle(id.begin(), id.end(), rng); }
        dim = 0;
        int b = 1;
        while (b < n && (integer ?
               p[id[b]].x == p[id[0]].x && p[id[b]].y == p[id[0]].y &&
               p[id[b]].z == p[id[0]].z : (p[id[b]] - p[id[0]]).abs() <= eps)) ++ b;
        if (b == n) return;
        swap(id[1], id[b]); dim = 1;
        int c = 2;
        while (c < n && line(id[0], id[1], id[c])) ++ c;
        if (c == n) return;
        swap(id[2], id[c]); dim = 2;
        int d = 3;
        while (d < n && ! side(id[0], id[1], id[2], id[d])) ++ d;
        if (d == n) return;
        swap(id[3], id[d]); dim = 3;
        inside = (p[id[0]] + p[id[1]] + p[id[2]] + p[id[3]]) / 4;
        V < Tri > tr;
        V < int > at, spare, next(n, -1), cnt(n);
        V < V < int > > conflict(n);
        auto make = [&](int a, int b, int c) {
            int t;
            if (spare.empty()) { t = tr.size(); tr.emplace_back(); }
            else { t = spare.back(); spare.pop_back(); }
            tr[t].v = {a, b, c}; tr[t].e = {-1, -1, -1}; tr[t].dead = 0;
            int f = at.size(); at.push_back(t);
            return f;
        };
        auto add = [&](int a, int b, int c, int d) {
            if (side(a, b, c, d) > 0) swap(b, c);
            make(a, b, c);
        };
        add(id[0], id[1], id[2], id[3]); add(id[0], id[1], id[3], id[2]);
        add(id[0], id[2], id[3], id[1]); add(id[1], id[2], id[3], id[0]);
        for (int f = 0; f < 4; ++ f) for (int e = 0; e < 3; ++ e)
            for (int g = 0; g < 4; ++ g) for (int k = 0; k < 3; ++ k)
                if (tr[f].v[e] == tr[g].v[(k + 1) % 3] &&
                    tr[f].v[(e + 1) % 3] == tr[g].v[k]) tr[f].e[e] = g * 3 + k;
        for (int f = 0; f < 4; ++ f) {
            auto v = tr[f].v; Plane eq = plane(v[0], v[1], v[2]);
            for (int i = 4; i < n; ++ i) {
                if (side(eq, id[i]) > 0) {
                    tr[f].pts.push_back(i); conflict[i].push_back(f); ++ cnt[i];
                }
            }
        }
        for (int i = 4; i < n; ++ i) {
            V < int > dead, fresh;
            for (int f : conflict[i])
                if (at[f] >= 0 && ! tr[at[f]].dead) {
                    dead.push_back(f); tr[at[f]].dead = 1;
                    for (int j : tr[at[f]].pts) if (j > i) -- cnt[j];
                }
            V < int >().swap(conflict[i]);
            for (int f : dead) {
                int t = at[f]; auto v = tr[t].v, e = tr[t].e;
                for (int k = 0; k < 3; ++ k) {
                    int g = e[k] / 3, s = e[k] % 3;
                    if (tr[at[g]].dead) continue;
                    int a = v[k], b = v[(k + 1) % 3];
                    if (line(a, b, id[i])) { slow(id); return; }
                    int h = make(a, b, id[i]), u = at[h];
                    fresh.push_back(h); next[a] = h;
                    tr[u].e[0] = e[k]; tr[at[g]].e[s] = h * 3;
                    Plane eq = plane(a, b, id[i]);
                    auto &x = tr[t].pts, &y = tr[at[g]].pts;
                    int l = upper_bound(x.begin(), x.end(), i) - x.begin();
                    int r = upper_bound(y.begin(), y.end(), i) - y.begin();
                    while (l < (int)x.size() || r < (int)y.size()) {
                        int j;
                        if (r == (int)y.size() || (l < (int)x.size() && x[l] < y[r])) j = x[l ++];
                        else if (l == (int)x.size() || y[r] < x[l]) j = y[r ++];
                        else { j = x[l ++]; ++ r; }
                        if (side(eq, id[j]) <= 0) continue;
                        tr[u].pts.push_back(j);
                        auto &q = conflict[j];
                        if ((int)q.size() > 2 * cnt[j] + 32) {
                            int z = 0;
                            for (int w : q)
                                if (at[w] >= 0 && ! tr[at[w]].dead) q[z ++] = w;
                            q.resize(z);
                        }
                        q.push_back(h); ++ cnt[j];
                    }
                }
            }
            for (int f : fresh) {
                int g = next[tr[at[f]].v[1]];
                tr[at[f]].e[1] = g * 3 + 2; tr[at[g]].e[2] = f * 3 + 1;
            }
            for (int f : dead) {
                int t = at[f]; at[f] = -1;
                V < int >().swap(tr[t].pts); spare.push_back(t);
            }
        }
        for (Tri &t : tr) if (! t.dead) faces.push_back({t.v[0], t.v[1], t.v[2]});
    }

    db area() const {
        db ans = 0;
        for (Face f : faces) ans += normal(f).abs() / 2;
        return ans;
    }
    db volume() const {
        db ans = 0;
        for (Face f : faces) ans += normal(f).dot(p[f.a] - inside) / 6;
        return fabsl(ans);
    }
};
