#include <bits/stdc++.h>
#include "../../template/others/单调栈与左右最近更小与更大.cpp"
#include "../../template/others/单调凸包与斜率优化.cpp"
#include "../../template/ds/左偏树与可并堆.cpp"
#include "../../template/math/快速幂与逆元.cpp"
#include "../../template/math/矩阵求逆.cpp"
#include "../../template/math/素性测试与整数分解.cpp"
#include "../../template/math/原根.cpp"

int main() {
    mt19937_64 rng(20261010);
    for (int test = 0; test < 3000; ++ test) {
        int n = int(rng() % 50);
        V < ll > a(n + 1);
        for (int i = 1; i <= n; ++ i) a[i] = int(rng() % 11) - 5;
        if (n && test % 7 == 0) a[1] = LLONG_MIN;
        if (n > 1 && test % 7 == 0) a[n] = LLONG_MAX;
        for (bool greater : {false, true}) {
            auto [left, right] = monotonic_bounds(a, greater);
            for (int i = 1; i <= n; ++ i) {
                auto good = [&](int j) { return greater ? a[j] > a[i] : a[j] < a[i]; };
                int l = i - 1, r = i + 1;
                while (l && ! good(l)) -- l;
                while (r <= n && ! good(r)) ++ r;
                assert(left[i] == l && right[i] == r);
            }
        }
        CHT hull;
        V < pair < ll, ll > > lines;
        ll k = 100, x = -100;
        for (int op = 0; op < 100; ++ op) {
            if (lines.empty() || rng() % 2) {
                k -= rng() % 4;
                ll b = int(rng() % 201) - 100;
                hull.add(k, b); lines.push_back({k, b});
            } else {
                x += rng() % 4;
                __int128_t expected = __int128_t(1) << 120;
                for (auto [slope, intercept] : lines)
                    expected = min(expected, __int128_t(slope) * x + intercept);
                assert(hull.query(x) == expected);
            }
        }
        hull.clear(); hull.add(0, 7); assert(hull.query(0) == 7);
        hull.clear(); hull.add(0, 0);
        ll length = int(rng() % 20) + 1;
        V < ll > sum(n + 1);
        V < ll > dp(n + 1);
        for (int i = 1; i <= n; ++ i) {
            sum[i] = sum[i - 1] + int(rng() % 20) + 2;
            ll x = sum[i] - length - 1;
            dp[i] = hull.query(x) + x * x;
            __int128_t expected = __int128_t(1) << 120;
            for (int j = 0; j < i; ++ j) {
                __int128_t d = sum[i] - sum[j] - length - 1;
                expected = min(expected, dp[j] + d * d);
            }
            assert(dp[i] == expected);
            hull.add(-2 * sum[i], dp[i] + sum[i] * sum[i]);
        }

        LeftistHeap heap;
        V < int > roots(8);
        V < multiset < pair < ll, int > > > values(8);
        for (int op = 0; op < 200; ++ op) {
            int u = int(rng() % roots.size());
            if (rng() % 3 == 0) {
                int v = int(rng() % roots.size());
                if (u == v) continue;
                roots[u] = heap.merge(roots[u], roots[v]); roots[v] = 0;
                values[u].insert(values[v].begin(), values[v].end()); values[v].clear();
            } else if (rng() % 2 && roots[u]) {
                assert(heap.top(roots[u]) == values[u].begin()->first);
                assert(roots[u] == values[u].begin()->second);
                roots[u] = heap.pop(roots[u]); values[u].erase(values[u].begin());
            } else {
                ll val = int(rng() % 21) - 10;
                int id = heap.push(val);
                roots[u] = heap.merge(roots[u], id); values[u].insert({val, id});
            }
        }
        for (int u = 0; u < int(roots.size()); ++ u) {
            while (roots[u]) {
                assert(heap.top(roots[u]) == values[u].begin()->first);
                assert(roots[u] == values[u].begin()->second);
                roots[u] = heap.pop(roots[u]); values[u].erase(values[u].begin());
            }
            assert(values[u].empty());
        }
        heap.init(); assert(heap.push(3) == 1 && heap.top(1) == 3);
    }
    for (ll mod : {1LL, 2LL, 101LL, 1000000007LL, 2147483647LL}) {
        for (int test = 0; test < 500; ++ test) {
            ll a = static_cast<ll> (rng());
            int b = int(rng() % 81);
            __int128_t x = a % mod, expected = 1 % mod;
            for (int i = 0; i < b; ++ i) expected = expected * x % mod;
            if (expected < 0) expected += mod;
            assert(pow_mod(a, b, mod) == expected);
        }
    }
    assert(pow_mod(LLONG_MIN, 1, 2147483647LL) == 2147483645LL);
    for (ll p : {2LL, 3LL, 101LL, 998244353LL, 2147483647LL}) {
        int n = int(min(p - 1, 250LL));
        auto inv = linear_inverse(n, p);
        for (int i = 1; i <= n; ++ i) assert(__int128_t(i) * inv[i] % p == 1);
        for (int test = 0; test < 100; ++ test) {
            V < ll > a(rng() % 51);
            for (ll &x : a) {
                x = ll(rng() % (p - 1)) + 1;
                if (rng() % 2) x -= p;
            }
            auto invs = batch_inverse(a, p);
            for (int i = 0; i < int(a.size()); ++ i) {
                __int128_t value = __int128_t(a[i]) * invs[i] % p;
                if (value < 0) value += p;
                assert(value == 1);
            }
        }
    }
    for (int test = 0; test < 2000; ++ test) {
        int n = int(rng() % 7);
        int p = V < int > {2, 3, 101, 998244353, INT_MAX}[rng() % 5];
        V < V < ll > > a(n, V < ll > (n));
        for (auto &row : a) for (ll &x : row) x = int(rng() % 21) - 10;
        V < int > perm(n); iota(perm.begin(), perm.end(), 0);
        ll det = 0;
        do {
            ll prod = 1; int parity = 0;
            for (int i = 0; i < n; ++ i) {
                prod = prod * ((a[i][perm[i]] % p + p) % p) % p;
                for (int j = 0; j < i; ++ j) parity ^= perm[j] > perm[i];
            }
            det = (det + (parity ? -prod : prod) + p) % p;
        } while (next_permutation(perm.begin(), perm.end()));
        V < V < ll > > b;
        assert(matrix_inv(a, p, b) == (det != 0));
        if (! det) continue;
        for (int i = 0; i < n; ++ i) for (int j = 0; j < n; ++ j) {
            ll ab = 0, ba = 0;
            for (int k = 0; k < n; ++ k) {
                ab = (ab + a[i][k] * b[k][j]) % p;
                ba = (ba + b[i][k] * a[k][j]) % p;
            }
            if (ab < 0) ab += p;
            if (ba < 0) ba += p;
            assert(ab == (i == j) && ba == (i == j));
        }
    }
    for (ull p = 2; p <= 1000; ++ p) if (is_prime(p)) {
        ull g = primitive_root(p), x = 1;
        set < ull > values;
        for (ull i = 0; i < p - 1; ++ i) {
            values.insert(x); x = x * g % p;
        }
        assert(values.size() == p - 1 && x == 1);
        for (ull h = 1; h < g; ++ h) {
            ull y = 1, order = 0;
            do { y = y * h % p; order ++; } while (y != 1);
            assert(order < p - 1);
        }
    }
    for (ull p : {998244353ULL, 1000000007ULL, 2305843009213693951ULL,
                  18446744073709551557ULL}) {
        ull g = primitive_root(p);
        for (ull q : factor(p - 1)) assert(rho_pow(g, (p - 1) / q, p) != 1);
    }
    {
        CHT hull;
        hull.add(1000000000LL, -1000000000LL);
        hull.add(0, 1000000000LL);
        hull.add(-1000000000LL, -1000000000LL);
        for (ll x : {-1000000000LL, 0LL, 1000000000LL}) {
            __int128_t expected = -__int128_t(1000000000LL) * abs(x) - 1000000000LL;
            assert(hull.query(x) == expected);
        }
    }
    {
        CHT hull; hull.add(0, 0);
        for (ll i = 1; i <= 50000; ++ i) {
            ll sum = i * 101, x = sum - 101;
            assert(hull.query(x) + x * x == 0);
            hull.add(-2 * sum, sum * sum);
        }
    }
    cout << "priority templates: stack / CHT / heap / modular / inverse / root OK\n";
}
