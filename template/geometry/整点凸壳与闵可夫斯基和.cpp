/*
整数坐标的严格凸包、上下凸壳和凸集和，使用 Boost 定宽整数精确判向。输入各分量绝对值 <= 10^18。
hull 返回逆时针顶点，从字典序最小点起；chain 每个横坐标只留一个端点。
建壳 O(n log n)。sum 接收 hull 的结果，支持空集、点、线段，时间 O(n+m)。
*/

#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

#define V vector
#define fi first
#define se second

typedef long long ll;

using namespace std;

struct IntHull {
    using P = pair < ll, ll >;
    using Int = boost::multiprecision::int256_t;

    static P sub(P a, P b) { return {a.fi - b.fi, a.se - b.se}; }
    static P add(P a, P b) { return {a.fi + b.fi, a.se + b.se}; }
    static Int det(P a, P b) { return Int(a.fi) * b.se - Int(a.se) * b.fi; }

    static V < P > walk(const V < P > &a, bool upper, bool one_x) {
        V < P > h;
        for (P p : a) {
            if (one_x && !h.empty() && h.back().fi == p.fi) continue;
            while (h.size() >= 2) {
                auto t = det(sub(h.back(), h[h.size() - 2]), sub(p, h.back()));
                if (upper ? t < 0 : t > 0) break;
                h.pop_back();
            }
            h.push_back(p);
        }
        return h;
    }

    static V < P > chain(V < P > a, bool upper) {
        sort(a.begin(), a.end(), [upper](P u, P v) {
            return u.fi != v.fi ? u.fi < v.fi : (upper ? u.se > v.se : u.se < v.se);
        });
        return walk(a, upper, true);
    }

    static V < P > hull(V < P > a) {
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        if (a.size() < 3) return a;
        auto h = walk(a, false, false), hi = walk(a, true, false);
        for (int i = int(hi.size()) - 2; i > 0; i --) h.push_back(hi[i]);
        return h;
    }

    static V < P > sum(const V < P > &a, const V < P > &b) {
        if (a.empty() || b.empty()) return {};
        auto edges = [](const V < P > &h) {
            V < P > e;
            if (h.size() > 1) for (int i = 0; i < int(h.size()); i ++)
                e.push_back(sub(h[(i + 1) % h.size()], h[i]));
            return e;
        };
        auto e = edges(a), f = edges(b);
        auto side = [](P v) { return v.fi < 0 || (v.fi == 0 && v.se < 0); };
        P p = add(a[0], b[0]);
        V < P > h{p};
        size_t i = 0, j = 0;
        while (i < e.size() || j < f.size()) {
            int cmp = 0;
            if (i == e.size()) cmp = 1;
            else if (j == f.size()) cmp = -1;
            else if (side(e[i]) != side(f[j])) cmp = side(e[i]) ? 1 : -1;
            else {
                auto d = det(e[i], f[j]);
                cmp = (d < 0) - (d > 0);
            }
            if (cmp <= 0) p = add(p, e[i ++]);
            if (cmp >= 0) p = add(p, f[j ++]);
            if (i < e.size() || j < f.size()) h.push_back(p);
        }
        return h;
    }
};

/*
auto a = IntHull::hull({{0, 0}, {3, 0}, {0, 2}});
auto upper = IntHull::chain(a, true), lower = IntHull::chain(a, false);
auto h = IntHull::sum(a, a);
*/
