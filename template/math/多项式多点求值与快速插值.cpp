#include <bits/stdc++.h>

#define V vector

using namespace std;

// 先复制数学章的“多项式求逆、对数、指数、幂、开根与除法”中的 FPS。
// 固定模数 998244353，系数与采样点均为 0-based。
// build 保存采样点；evaluate 返回每点函数值，允许重复点。
// interpolate 要求采样点模意义下互异，返回次数 < n 的多项式，恰好 n 项。
// 输入系数、横纵坐标先归一化；点数与输入多项式长度均不超过 2^20。
// 时间 O((n + m) log^2(n + m))，空间 O(n log n + m)，支持空数组与重复 build。
struct Multipoint {
    static constexpr int mod = 998244353;
    int n = 0;
    V < int > x;
    V < V < int > > prod;

    void build_node(int p, int l, int r) {
        if (l == r) { prod[p] = {x[l] ? mod - x[l] : 0, 1}; return; }
        int mid = (l + r) / 2;
        build_node(p * 2, l, mid); build_node(p * 2 + 1, mid + 1, r);
        prod[p] = FPS::mul(prod[p * 2], prod[p * 2 + 1]);
    }

    void build(const V < int > &points) {
        x = points; n = int(x.size());
        prod.assign(4 * max(1, n), {});
        if (n) build_node(1, 0, n - 1);
    }

    V < int > remainder(const V < int > &a, const V < int > &b) const {
        return a.size() < b.size() ? a : FPS::divmod(a, b).second;
    }

    void eval_node(int p, int l, int r, const V < int > &a,
                   V < int > &ans) const {
        if (l == r) {
            int v = 0;
            for (int i = int(a.size()) - 1; i >= 0; -- i)
                v = (1ll * v * x[l] + a[i]) % mod;
            ans[l] = v; return;
        }
        int mid = (l + r) / 2;
        eval_node(p * 2, l, mid, remainder(a, prod[p * 2]), ans);
        eval_node(p * 2 + 1, mid + 1, r, remainder(a, prod[p * 2 + 1]), ans);
    }

    V < int > evaluate(const V < int > &a) const {
        V < int > ans(n);
        if (n) eval_node(1, 0, n - 1, remainder(a, prod[1]), ans);
        return ans;
    }

    V < int > interpolate_node(int p, int l, int r,
                               const V < int > &w) const {
        if (l == r) return {w[l]};
        int mid = (l + r) / 2;
        auto a = FPS::mul(interpolate_node(p * 2, l, mid, w), prod[p * 2 + 1]);
        auto b = FPS::mul(interpolate_node(p * 2 + 1, mid + 1, r, w), prod[p * 2]);
        a.resize(max(a.size(), b.size()));
        for (int i = 0; i < int(b.size()); ++ i) {
            a[i] += b[i];
            if (a[i] >= mod) a[i] -= mod;
        }
        return a;
    }

    V < int > interpolate(const V < int > &y) const {
        if (! n) return {};
        V < int > d(n), w(n);
        for (int i = 1; i <= n; ++ i) d[i - 1] = 1ll * i * prod[1][i] % mod;
        auto values = evaluate(d);
        for (int i = 0; i < n; ++ i) {
            assert(values[i]);
            w[i] = 1ll * y[i] * FPS::power(values[i], mod - 2) % mod;
        }
        auto ans = interpolate_node(1, 0, n - 1, w);
        ans.resize(n); return ans;
    }
};

// 调用：Multipoint poly; poly.build(x);
// auto y = poly.evaluate(a); auto f = poly.interpolate(y);
