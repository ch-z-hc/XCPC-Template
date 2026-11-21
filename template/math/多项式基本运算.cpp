#include <bits/stdc++.h>

#define V vector

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

/*
固定模 998244353。a[i] 是 x^i 的系数，先化到 [0, mod)。以下运算取前 n 项，n <= 2^22。
inv 要求常数项非零，ln 要求为 1，exp 要求为 0（也可传空数组）。pow 的指数非负，0^0 取 1。
sqrt 无解返回 false，有解时写入 ans，首个非零系数的根取较小者。n = 0 时结果为空。
divmod 返回 {商, 余数}，去掉高次零，零多项式用空数组表示。除数须非零，两端长度均不超过 2^22。
乘法结果长度须 <= 2^23。上述运算时间 O(n log n)，空间 O(n)。
*/

struct FPS {
    static const int mod = 998244353;

    static int power(int a, int b) {
        int ans = 1;
        for (; b; b >>= 1, a = 1ll * a * a % mod)
            if (b & 1) ans = 1ll * ans * a % mod;
        return ans;
    }

    static void ntt(V < int > &a, bool inv = false) {
        int n = int(a.size());
        for (int i = 1, j = 0; i < n; ++ i) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            int wn = power(3, (mod - 1) / len);
            if (inv) wn = power(wn, mod - 2);
            for (int l = 0; l < n; l += len) {
                ll w = 1;
                for (int i = 0; i < len / 2; ++ i, w = w * wn % mod) {
                    int x = a[l + i], y = w * a[l + i + len / 2] % mod;
                    a[l + i] = x + y;
                    if (a[l + i] >= mod) a[l + i] -= mod;
                    a[l + i + len / 2] = x - y;
                    if (a[l + i + len / 2] < 0) a[l + i + len / 2] += mod;
                }
            }
        }
        if (inv) {
            int v = power(n, mod - 2);
            for (int &x : a) x = 1ll * x * v % mod;
        }
    }

    static V < int > mul(V < int > a, V < int > b) {
        if (a.empty() || b.empty()) return {};
        int m = int(a.size() + b.size() - 1), n = 1;
        while (n < m) n <<= 1;
        assert(n <= (1 << 23));
        a.resize(n); b.resize(n);
        ntt(a); ntt(b);
        for (int i = 0; i < n; ++ i) a[i] = 1ll * a[i] * b[i] % mod;
        ntt(a, true);
        a.resize(m);
        return a;
    }

    static V < int > inv(const V < int > &a, int n) {
        if (! n) return {};
        assert(n <= (1 << 22) && ! a.empty() && a[0]);
        V < int > b{power(a[0], mod - 2)};
        for (int len = 2; int(b.size()) < n; len <<= 1) {
            int m = min(len, n);
            V < int > c(a.begin(), a.begin() + min(m, int(a.size())));
            c = mul(c, b); c.resize(m);
            for (int &x : c) x = x ? mod - x : 0;
            c[0] = (c[0] + 2) % mod;
            b = mul(b, c); b.resize(m);
        }
        return b;
    }

    static V < int > ln(const V < int > &a, int n) {
        if (! n) return {};
        assert(n <= (1 << 22) && ! a.empty() && a[0] == 1);
        int m = min(n, int(a.size()));
        V < int > d(max(0, m - 1));
        for (int i = 1; i < m; ++ i) d[i - 1] = 1ll * i * a[i] % mod;
        d = mul(d, inv(a, n)); d.resize(n - 1);
        V < int > b(n), iv(n, 1);
        for (int i = 2; i < n; ++ i)
            iv[i] = mod - 1ll * (mod / i) * iv[mod % i] % mod;
        for (int i = 1; i < n; ++ i) b[i] = 1ll * d[i - 1] * iv[i] % mod;
        return b;
    }

    static V < int > exp(const V < int > &a, int n) {
        if (! n) return {};
        assert(n <= (1 << 22) && (a.empty() || a[0] == 0));
        V < int > b{1};
        for (int len = 2; int(b.size()) < n; len <<= 1) {
            int m = min(len, n);
            V < int > c = ln(b, m);
            for (int i = 0; i < m; ++ i) {
                c[i] = (i < int(a.size()) ? a[i] : 0) - c[i];
                if (c[i] < 0) c[i] += mod;
            }
            c[0] ++;
            b = mul(b, c); b.resize(m);
        }
        return b;
    }

    static V < int > pow(const V < int > &a, ull k, int n) {
        if (! n) return {};
        assert(n <= (1 << 22));
        V < int > ans(n);
        if (! k) { ans[0] = 1; return ans; }
        int p = 0, m = min(n, int(a.size()));
        while (p < m && ! a[p]) p ++;
        if (p == m || (p && k > ull((n - 1) / p)))
            return ans;
        int shift = int(p * k), len = n - shift;
        int iv = power(a[p], mod - 2);
        V < int > b(len);
        for (int i = 0; i < len && i + p < m; ++ i)
            b[i] = 1ll * a[i + p] * iv % mod;
        b = ln(b, len);
        for (int &x : b) x = x * (k % mod) % mod;
        b = exp(b, len);
        int v = power(a[p], int(k % (mod - 1)));
        for (int i = 0; i < len; ++ i)
            ans[i + shift] = 1ll * b[i] * v % mod;
        return ans;
    }

    static int sqrt_mod(int a) {
        if (! a) return 0;
        if (power(a, (mod - 1) / 2) != 1) return -1;
        int q = (mod - 1) >> 23, s = 23;
        int c = power(3, q), t = power(a, q);
        int r = power(a, (q + 1) / 2);
        while (t != 1) {
            int i = 0, v = t;
            while (v != 1) v = 1ll * v * v % mod, i ++;
            int b = power(c, 1 << (s - i - 1));
            r = 1ll * r * b % mod;
            c = 1ll * b * b % mod;
            t = 1ll * t * c % mod;
            s = i;
        }
        return min(r, mod - r);
    }

    static bool sqrt(const V < int > &a, int n, V < int > &ans) {
        if (! n) { ans.clear(); return true; }
        assert(n <= (1 << 22));
        int p = 0, m = min(n, int(a.size()));
        while (p < m && ! a[p]) p ++;
        if (p == m) { ans.assign(n, 0); return true; }
        if (p & 1) return false;
        int r = sqrt_mod(a[p]);
        if (r == -1) return false;
        int len = n - p;
        V < int > c(len), b{r};
        for (int i = 0; i < len && i + p < m; ++ i) c[i] = a[i + p];
        for (int size = 2; int(b.size()) < len; size <<= 1) {
            int k = min(size, len);
            V < int > d(c.begin(), c.begin() + k);
            d = mul(d, inv(b, k)); d.resize(k);
            b.resize(k);
            for (int i = 0; i < k; ++ i)
                b[i] = 1ll * (b[i] + d[i]) * ((mod + 1) / 2) % mod;
        }
        ans.assign(n, 0);
        for (int i = 0; i < len; ++ i) ans[i + p / 2] = b[i];
        return true;
    }

    static pair < V < int >, V < int > > divmod(V < int > a,
                                               V < int > b) {
        assert(a.size() <= (1 << 22) && b.size() <= (1 << 22));
        while (! a.empty() && ! a.back()) a.pop_back();
        while (! b.empty() && ! b.back()) b.pop_back();
        assert(! b.empty());
        if (a.size() < b.size()) return {{}, a};
        int n = int(a.size()), m = int(b.size()), k = n - m + 1;
        V < int > ar = a, br = b;
        reverse(ar.begin(), ar.end()); reverse(br.begin(), br.end());
        ar.resize(k);
        V < int > q = mul(ar, inv(br, k)); q.resize(k);
        reverse(q.begin(), q.end());
        V < int > r = mul(q, b); r.resize(m - 1);
        for (int i = 0; i < m - 1; ++ i) {
            r[i] = a[i] - r[i];
            if (r[i] < 0) r[i] += mod;
        }
        while (! q.empty() && ! q.back()) q.pop_back();
        while (! r.empty() && ! r.back()) r.pop_back();
        return {q, r};
    }
};
const int FPS::mod;

/*
V < int > a{1, 2, 3};
auto inv = FPS::inv(a, 8); // 保留到 x^7
V < int > root; bool ok = FPS::sqrt(a, 8, root);
auto [q, r] = FPS::divmod(a, {1, 1});
*/
