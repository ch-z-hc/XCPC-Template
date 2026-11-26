/*
先复制「多项式基本运算」。固定模 998244353，低次系数在前，点和系数先取模。
求值可用重复点，插值的横坐标须模意义下互异。点数 n、系数数 m 均 <= 2^20。
时间 O((n + m) log²(n + m))，乘积树空间 O(n log n + m)。
*/

#include <bits/stdc++.h>

struct Multipoint {
    static const int mod = 998244353;
    int n = 0;
    std::vector<int> x;
    std::vector<std::vector<int>> prod;

    void build(int p, int l, int r) {
        if (l == r) {
            prod[p] = {x[l] ? mod - x[l] : 0, 1};
            return;
        }
        int mid = (l + r) / 2;
        build(p * 2, l, mid);
        build(p * 2 + 1, mid + 1, r);
        prod[p] = FPS::mul(prod[p * 2], prod[p * 2 + 1]);
    }

    void build(const std::vector<int> &points) {
        x = points;
        n = int(x.size());
        prod.assign(4 * std::max(1, n), {});
        if (n) build(1, 0, n - 1);
    }

    std::vector<int> rem(const std::vector<int> &a, const std::vector<int> &b) {
        return a.size() < b.size() ? a : FPS::divmod(a, b).second;
    }

    void eval(int p, int l, int r, const std::vector<int> &a, std::vector<int> &ans) {
        if (l == r) {
            int v = 0;
            for (int i = int(a.size()) - 1; i >= 0; i --)
                v = (1ll * v * x[l] + a[i]) % mod;
            ans[l] = v;
            return;
        }
        int mid = (l + r) / 2;
        eval(p * 2, l, mid, rem(a, prod[p * 2]), ans);
        eval(p * 2 + 1, mid + 1, r, rem(a, prod[p * 2 + 1]), ans);
    }

    std::vector<int> eval(const std::vector<int> &a) {
        std::vector<int> ans(n);
        if (n) eval(1, 0, n - 1, rem(a, prod[1]), ans);
        return ans;
    }

    std::vector<int> interp(int p, int l, int r, const std::vector<int> &w) {
        if (l == r) return {w[l]};
        int mid = (l + r) / 2;
        auto a = FPS::mul(interp(p * 2, l, mid, w), prod[p * 2 + 1]);
        auto b = FPS::mul(interp(p * 2 + 1, mid + 1, r, w), prod[p * 2]);
        a.resize(std::max(a.size(), b.size()));
        for (int i = 0; i < int(b.size()); i ++) {
            a[i] += b[i];
            if (a[i] >= mod) a[i] -= mod;
        }
        return a;
    }

    std::vector<int> interp(const std::vector<int> &y) {
        if (!n) return {};
        std::vector<int> d(n), w(n);
        for (int i = 1; i <= n; i ++) d[i - 1] = 1ll * i * prod[1][i] % mod;
        auto values = eval(d);
        for (int i = 0; i < n; i ++) {
            assert(values[i]);
            w[i] = 1ll * y[i] * FPS::power(values[i], mod - 2) % mod;
        }
        auto ans = interp(1, 0, n - 1, w);
        ans.resize(n);
        return ans;
    }
};
const int Multipoint::mod;

/*
Multipoint poly; poly.build(x);
auto y = poly.eval(a); auto f = poly.interp(y);
*/
