#include <bits/stdc++.h>
#include <cassert>
#include "../../template/math/FloorSum.cpp"
#include "../../template/math/ModSqrt.cpp"
#include "../../template/math/DuJiao.cpp"
#include "../../template/math/ExLucas.cpp"
#include "../../template/math/Determinant.cpp"
#include "../../template/math/FPS.cpp"
#include "../../template/math/XorBasis.cpp"

using namespace std;

mt19937_64 rng(2026101037);

ll power_ref(ll a, ll k, ll mod) {
    ll ans = 1;
    for (; k; k >>= 1, a = a * a % mod)
        if (k & 1) ans = ans * a % mod;
    return ans;
}

i128 floor_ref(i128 a, i128 b) {
    i128 q = a / b;
    if (a % b < 0) q --;
    return q;
}

void check_floor_sum() {
    for (int t = 0; t < 3000; ++ t) {
        ll n = rng() % 70, m = 1 + rng() % 100;
        ll a = ll(rng() % 2001) - 1000, b = ll(rng() % 2001) - 1000;
        i128 ans = 0;
        for (ll i = 0; i < n; ++ i) ans += floor_ref(i128(a) * i + b, m);
        assert(floor_sum(n, m, a, b) == ans);
    }
    for (ll a : {LLONG_MIN, LLONG_MAX, 0LL})
        for (ll b : {LLONG_MIN, LLONG_MAX, 0LL})
            for (ll m : {1LL, 2LL, LLONG_MAX}) {
                i128 ans = 0;
                for (ll i = 0; i < 5; ++ i)
                    ans += floor_ref(i128(a) * i + b, m);
                assert(floor_sum(5, m, a, b) == ans);
            }
    ll n = LLONG_MAX;
    assert(floor_sum(n, n, n, 0) == i128(n) * (n - 1) / 2);
    assert(floor_sum(0, 1, LLONG_MIN, LLONG_MIN) == 0);
}

void check_mod_sqrt() {
    V < int > primes{2, 3, 5, 7, 11, 13, 17, 19, 31, 97, 257};
    for (int t = 0; t < 3000; ++ t) {
        int p = primes[rng() % primes.size()];
        ll a = ll(rng() % 5001) - 2500, v = (a % p + p) % p;
        ll ans = -1;
        for (ll x = 0; x < p; ++ x)
            if (x * x % p == v) { ans = x; break; }
        assert(sqrt_mod(a, p) == ans);
    }
    for (ll p : {998244353LL, 1000000007LL, 2147483647LL}) {
        assert(sqrt_mod(0, p) == 0);
        assert((sqrt_mod(LLONG_MIN, p) != -1) ==
               (power_ref((LLONG_MIN % p + p) % p, (p - 1) / 2, p) == 1));
        for (int t = 0; t < 100; ++ t) {
            ll x = rng() % p;
            assert(sqrt_mod(x * x % p, p) == min(x, p - x));
        }
    }
}

void check_dujiao() {
    const int n = 1000;
    V < ll > phi(n + 1), mu(n + 1);
    for (int i = 1; i <= n; ++ i) {
        for (int j = 1; j <= i; ++ j) phi[i] += gcd(i, j) == 1;
        int x = i, cnt = 0, sign = 1;
        for (int p = 2; p <= x; ++ p) if (x % p == 0) {
            int e = 0;
            while (x % p == 0) x /= p, e ++;
            cnt ++;
            if (e > 1) sign = 0;
        }
        mu[i] = sign * (cnt & 1 ? -1 : 1);
        phi[i] += phi[i - 1]; mu[i] += mu[i - 1];
    }
    DuJiao d;
    for (int t = 0; t < 3000; ++ t) {
        if (t % 100 == 0) d.init(1 + rng() % 50);
        int x = rng() % (n + 1);
        assert(d.phi(x) == phi[x] && d.mu(x) == mu[x]);
        assert(d.phi(x) == phi[x] && d.mu(x) == mu[x]);
    }
    d.init(n);
    for (int x = 0; x <= n; ++ x)
        assert(d.phi(x) == phi[x] && d.mu(x) == mu[x]);
    d.init(1);
    assert(d.phi(0) == 0 && d.mu(0) == 0);
    assert(d.phi(n) == phi[n] && d.mu(n) == mu[n]);
}

void check_exlucas() {
    i128 c[71][71]{};
    for (int n = 0; n <= 70; ++ n) {
        c[n][0] = c[n][n] = 1;
        for (int k = 1; k < n; ++ k) c[n][k] = c[n - 1][k - 1] + c[n - 1][k];
    }
    ExLucas e;
    for (int t = 0; t < 3000; ++ t) {
        int mod = 1 + rng() % 400, n = rng() % 71;
        int k = int(rng() % (n + 5)) - 2;
        e.init(mod);
        ll ans = k < 0 || k > n ? 0 : ll(c[n][k] % mod);
        assert(e.choose(n, k) == ans);
        assert(e.choose(0, 0) == 1 % mod);
        assert(e.choose(n, -1) == 0 && e.choose(n, n + 1) == 0);
        ll big = LLONG_MAX;
        assert(e.choose(big, 0) == 1 % mod);
        assert(e.choose(big, 1) == big % mod);
        assert(e.choose(big, big - 1) == big % mod);
        assert(e.choose(big, 2) == i128(big) * (big - 1) / 2 % mod);
    }
}

ll det_ref(const V < V < ll > > &a, ll mod) {
    int n = int(a.size());
    V < int > p(n); iota(p.begin(), p.end(), 0);
    ll ans = 0;
    do {
        ll val = 1; int parity = 0;
        for (int i = 0; i < n; ++ i) {
            val = val * ((a[i][p[i]] % mod + mod) % mod) % mod;
            for (int j = i + 1; j < n; ++ j) parity ^= p[i] > p[j];
        }
        ans = (ans + (parity ? -val : val) + mod) % mod;
    } while (next_permutation(p.begin(), p.end()));
    return ans;
}

ll tree_ref(int n, const V < tuple < int, int, ll > > &edges,
            ll mod, int root, bool directed) {
    ll ans = 0;
    int m = int(edges.size());
    for (int s = 0; s < (1 << m); ++ s) {
        if (__builtin_popcount(unsigned(s)) != n - 1) continue;
        V < V < int > > g(n + 1);
        V < int > in(n + 1);
        ll val = 1;
        for (int i = 0; i < m; ++ i) if (s >> i & 1) {
            auto [u, v, w] = edges[i];
            val = val * ((w % mod + mod) % mod) % mod;
            g[u].push_back(v); in[v] ++;
            if (! directed) g[v].push_back(u);
        }
        bool ok = true;
        if (directed) for (int i = 1; i <= n; ++ i)
            if (in[i] != (i != root)) ok = false;
        V < bool > seen(n + 1);
        queue < int > q; q.push(root); seen[root] = true;
        while (! q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) if (! seen[v]) seen[v] = true, q.push(v);
        }
        for (int i = 1; i <= n; ++ i) if (! seen[i]) ok = false;
        if (ok) ans = (ans + val) % mod;
    }
    return ans;
}

void check_determinant() {
    V < ll > primes{2, 3, 101, 998244353, 2147483647};
    for (int t = 0; t < 2500; ++ t) {
        int n = rng() % 7;
        ll mod = primes[rng() % primes.size()];
        V < V < ll > > a(n, V < ll > (n));
        for (auto &row : a) for (ll &x : row) x = ll(rng() % 101) - 50;
        assert(det(a, mod) == det_ref(a, mod));
        int v = 1 + rng() % 5, m = rng() % 9, root = 1 + rng() % v;
        V < tuple < int, int, ll > > edges;
        for (int i = 0; i < m; ++ i)
            edges.emplace_back(1 + rng() % v, 1 + rng() % v, ll(rng() % 7) - 2);
        for (bool directed : {false, true})
            assert(matrix_tree(v, edges, mod, root, directed) ==
                   tree_ref(v, edges, mod, root, directed));
    }
    assert(det({{LLONG_MIN}}, 2147483647) ==
           (LLONG_MIN % 2147483647 + 2147483647) % 2147483647);
}

V < int > mul_ref(const V < int > &a, const V < int > &b, int n) {
    V < int > c(n);
    for (int i = 0; i < int(a.size()) && i < n; ++ i)
        for (int j = 0; j < int(b.size()) && i + j < n; ++ j)
            c[i + j] = (c[i + j] + 1ll * a[i] * b[j]) % FPS::mod;
    return c;
}

void trim(V < int > &a) {
    while (! a.empty() && ! a.back()) a.pop_back();
}

pair < V < int >, V < int > > div_ref(V < int > a, V < int > b) {
    trim(a); trim(b);
    if (a.size() < b.size()) return {{}, a};
    int k = int(a.size() - b.size() + 1), m = int(b.size());
    V < int > q(k);
    ll iv = power_ref(b.back(), FPS::mod - 2, FPS::mod);
    for (int i = k - 1; i >= 0; -- i) {
        q[i] = a[i + m - 1] * iv % FPS::mod;
        for (int j = 0; j < m; ++ j) {
            a[i + j] = (a[i + j] - 1ll * q[i] * b[j]) % FPS::mod;
            if (a[i + j] < 0) a[i + j] += FPS::mod;
        }
    }
    trim(a); trim(q);
    return {q, a};
}

void check_fps() {
    const int mod = FPS::mod;
    for (int t = 0; t < 2500; ++ t) {
        int n = rng() % 25, k = rng() % 8;
        V < int > a(rng() % 26);
        for (int &x : a) x = rng() % mod;
        if (t % 3 == 0) {
            int z = rng() % (a.size() + 1);
            fill(a.begin(), a.begin() + z, 0);
        }
        V < int > ans(n);
        if (n) ans[0] = 1;
        for (int i = 0; i < k; ++ i) ans = mul_ref(ans, a, n);
        assert(FPS::pow(a, k, n) == ans);
        V < int > s = t & 1 ? mul_ref(a, a, n) : a;
        s.resize(n);
        int p = 0;
        while (p < n && ! s[p]) p ++;
        bool exists = p == n || (!(p & 1) &&
                      power_ref(s[p], (mod - 1) / 2, mod) == 1);
        V < int > root;
        bool ok = FPS::sqrt(s, n, root);
        assert(ok == exists);
        if (ok) {
            assert(int(root.size()) == n);
            assert(mul_ref(root, root, n) == s);
            if (p < n) assert(root[p / 2] <= mod - root[p / 2]);
        }
        V < int > b(1 + rng() % 25);
        for (int &x : b) x = rng() % mod;
        b.back() = 1 + rng() % (mod - 1);
        if (t % 2 == 0) b.push_back(0), a.push_back(0);
        assert(FPS::divmod(a, b) == div_ref(a, b));
    }
    for (unsigned long long k : {0ULL, 1ULL, 998244353ULL, ULLONG_MAX}) {
        assert(FPS::pow({2}, k, 5)[0] == power_ref(2, k % (mod - 1), mod));
        assert(FPS::pow({}, k, 0).empty());
    }
    for (int t = 0; t < 100; ++ t) {
        unsigned long long k = rng(), e = k;
        int n = 1 + rng() % 20;
        V < int > a(n), ans(n), cur;
        for (int &x : a) x = rng() % mod;
        ans[0] = 1; cur = a;
        for (; e; e >>= 1, cur = mul_ref(cur, cur, n))
            if (e & 1) ans = mul_ref(ans, cur, n);
        assert(FPS::pow(a, k, n) == ans);
    }
    assert(FPS::pow({0, 1}, ULLONG_MAX, 20) == V < int > (20));
    assert(FPS::pow({}, 0, 3) == V < int > ({1, 0, 0}));
    V < int > root;
    assert(FPS::sqrt({}, 3, root) && root == V < int > (3));
    assert(FPS::sqrt({}, 0, root) && root.empty());
    assert(! FPS::sqrt({0, 1}, 5, root));
    assert(! FPS::sqrt({3}, 1, root));
    assert(FPS::divmod({}, {1}) == div_ref({}, {1}));
}

void check_basis() {
    Basis b;
    unsigned long long ans;
    for (int t = 0; t < 2500; ++ t) {
        b.clear();
        V < unsigned long long > a{0};
        int n = rng() % 10, shift = t & 1 ? 54 : 0;
        for (int i = 0; i < n; ++ i) {
            auto x = (rng() % 512) << shift;
            bool exists = find(a.begin(), a.end(), x) != a.end();
            assert(b.insert(x) != exists);
            int m = int(a.size());
            for (int j = 0; j < m; ++ j) a.push_back(a[j] ^ x);
            sort(a.begin(), a.end());
            a.erase(unique(a.begin(), a.end()), a.end());
        }
        for (int i = 0; i < 15; ++ i) {
            unsigned long long k = rng() % (a.size() + 2);
            assert(b.kth(k, ans) == (k < a.size()));
            if (k < a.size()) assert(ans == a[k]);
            auto x = (rng() % 512) << shift;
            assert(b.contains(x) == binary_search(a.begin(), a.end(), x));
            unsigned long long best = 0;
            for (auto v : a) best = max(best, v ^ x);
            assert(b.max_xor(x) == best);
        }
        assert(b.kth(0, ans) && ans == 0);
        assert(! b.kth(a.size(), ans));
    }
    b.clear();
    for (int i = 0; i < 63; ++ i) assert(b.insert(1ULL << i));
    for (int t = 0; t < 2500; ++ t) {
        auto k = rng() & LLONG_MAX;
        assert(b.kth(k, ans) && ans == k);
    }
    assert(b.kth(LLONG_MAX, ans) && ans == LLONG_MAX);
    assert(! b.kth(1ULL << 63, ans) && ! b.kth(ULLONG_MAX, ans));
    assert(! b.insert(0) && b.max_xor() == (unsigned long long)LLONG_MAX);
    b.clear();
    assert(b.max_xor() == 0 && ! b.kth(1, ans));
}

int main() {
    check_floor_sum(); check_mod_sqrt(); check_dujiao(); check_exlucas();
    check_determinant(); check_fps(); check_basis();
    cout << "extra math random: OK\n";
}
