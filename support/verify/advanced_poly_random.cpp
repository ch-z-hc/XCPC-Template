#include <bits/stdc++.h>
#include "../../template/math/多项式基本运算.cpp"
#include "../../template/math/多项式多点求值与快速插值.cpp"
#include "../../template/math/子集卷积.cpp"

int main() {
    mt19937 rng(20261011);
    for (int test = 0; test < 2000; ++ test) {
        int n = 1 << (rng() % 8);
        int mod = V < int > {1, 2, 4, 1000, 998244353, INT_MAX}[rng() % 6];
        V < int > a(n), b(n), expected(n);
        for (int &x : a) x = int(rng() % 2001) - 1000;
        for (int &x : b) x = int(rng() % 2001) - 1000;
        if (test % 7 == 0) a[0] = INT_MIN, b[0] = INT_MAX;
        for (int s = 0; s < n; ++ s) {
            int t = s;
            do {
                long long value = (expected[s] + 1ll * a[t] * b[s ^ t]) % mod;
                if (value < 0) value += mod;
                expected[s] = int(value);
                if (! t) break;
                t = (t - 1) & s;
            } while (true);
        }
        assert(subset_convolution(a, b, mod) == expected);
    }
    const int mod = FPS::mod;
    Multipoint poly;
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 25), m = int(rng() % 36);
        V < int > a(m), x(n), expected(n), y(n);
        for (int &v : a) v = rng() % mod;
        for (int &v : x) v = rng() % 12;
        for (int i = 0; i < n; ++ i)
            for (int j = m - 1; j >= 0; -- j)
                expected[i] = (1ll * expected[i] * x[i] + a[j]) % mod;
        poly.build(x); assert(poly.evaluate(a) == expected);
        assert(poly.evaluate({}) == V < int > (n));
        poly.build(x); assert(poly.evaluate(a) == expected);
        iota(x.begin(), x.end(), 0); shuffle(x.begin(), x.end(), rng);
        for (int &v : y) v = rng() % mod;
        poly.build(x);
        auto got = poly.interpolate(y);
        V < int > coefficients(n);
        for (int i = 0; i < n; ++ i) {
            V < int > basis{1}; long long den = 1;
            for (int j = 0; j < n; ++ j) if (j != i) {
                V < int > next(basis.size() + 1);
                for (int k = 0; k < int(basis.size()); ++ k) {
                    next[k] = (next[k] - 1ll * basis[k] * x[j] % mod + mod) % mod;
                    next[k + 1] = (next[k + 1] + basis[k]) % mod;
                }
                basis = move(next); den = den * (x[i] - x[j] + mod) % mod;
            }
            long long w = 1ll * y[i] * FPS::power(int(den), mod - 2) % mod;
            for (int j = 0; j < n; ++ j)
                coefficients[j] = (coefficients[j] + w * basis[j]) % mod;
        }
        assert(got == coefficients);
        assert(poly.evaluate(got) == y);
    }
    V < int > x(4096), a(5000);
    iota(x.begin(), x.end(), 0); a[0] = 5; a[1] = 2; a[3] = 1;
    poly.build(x); auto y = poly.evaluate(a);
    for (int i = 0; i < int(x.size()); ++ i)
        assert(y[i] == (1ll * i * i % mod * i + 2 * i + 5) % mod);
    a.resize(x.size()); assert(poly.interpolate(y) == a);
    cout << "advanced polynomial: subset / multipoint / interpolation OK\n";
}
