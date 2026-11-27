#include "../../template/others/稀疏表.cpp"
#include "../../template/math/高精度整数与分数.cpp"
#include "../../template/math/宽整数模乘.cpp"
#include "../../template/math/固定底数快速幂.cpp"
#include "../../template/math/二进制最大公约数与位筛.cpp"
#include "../../template/math/值域预处理最大公约数.cpp"
#include "../../template/math/任意模数原根枚举.cpp"
#include "../../template/math/多维整除分块.cpp"
#include "../../template/others/公历日期换算.cpp"
#include "../../template/others/整数取整与前缀异或.cpp"
#include "../../template/others/随机抽样与有标号树.cpp"
#include "../../template/others/缓冲整数读写.cpp"
#include "../../template/geometry/整点凸壳与闵可夫斯基和.cpp"

int main() {
    GcdTable table;
    table.init(2000);
    for (int a = 0; a <= 2000; a ++) for (int b = 0; b <= 2000; b ++) {
        assert(table.query(a, b) == gcd(a, b));
        assert(binary_gcd(a, b) == unsigned(gcd(a, b)));
    }
    auto prime = prime_flags(2000);
    for (int n = 0; n <= 2000; n ++) {
        bool ok = n >= 2;
        for (int d = 2; d * d <= n; d ++) if (n % d == 0) ok = false;
        assert(prime[n] == ok);
    }
    for (int n = 1; n <= 150; n ++) {
        V < int > roots;
        for (int a = 0; a < n; a ++) if (gcd(a, n) == 1) {
            set < int > powers;
            for (int x = 1 % n; powers.insert(x).se; x = x * a % n) {}
            bool ok = true;
            for (int k = 0; k < n; k ++) if (gcd(k, n) == 1 && !powers.count(k)) ok = false;
            if (ok) roots.push_back(a);
        }
        assert(all_roots(n) == roots);
    }
    FixedPower f;
    for (int mod : {1, 2, 17, INT_MAX}) {
        f.init(-9, 1234, mod);
        ll x = 1 % mod;
        for (int i = 0; i <= 1234; i ++) {
            assert(f.pow(i) == x);
            x = (mod - 9ll % mod) * x % mod;
        }
    }
    for (int a = -50; a <= 50; a ++) for (int b = -30; b <= 30; b ++) if (b) {
        assert(floor_div(a, b) == floor(double(a) / b));
        assert(ceil_div(a, b) == ceil(double(a) / b));
    }
    ull xr = 0;
    for (ull n = 0; n <= 10000; n ++) {
        xr ^= n;
        assert(prefix_xor(n) == xr);
        ull q = int_sqrt(n);
        assert(q * q <= n && (q + 1) * (q + 1) > n);
        assert(int_sqrt(n, true) == q + (q * q != n));
        for (ull a = 2; n && a < 15; a ++) {
            ull v = 1; int k = 0;
            while (v * a <= n) v *= a, k ++;
            assert(int_log(a, n) == k && int_log(a, n, true) == k + (v != n));
        }
    }
    assert(int_sqrt(ULLONG_MAX, true) == (1ull << 32));
    assert(day_number(1, 1, 1) == 0);
    int day = 0;
    for (int y = 1; y <= 800; y ++) for (int m = 1; m <= 12; m ++) {
        int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        days[2] += y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
        for (int d = 1; d <= days[m]; d ++, day ++) {
            array < int, 3 > expected{y, m, d};
            assert(day_number(y, m, d) == day && civil_date(day) == expected);
        }
    }
    Sampler rng(20261127);
    for (int n = 1; n <= 60; n ++) {
        for (int k : {0, n / 2, n}) {
            auto a = rng.sample(n, k);
            assert(int(a.size()) == k && int(set < int >(a.begin(), a.end()).size()) == k);
            for (int x : a) assert(0 <= x && x < n);
        }
        auto e = rng.tree(n);
        assert(int(e.size()) == n - 1);
        V < int > id(n + 1); iota(id.begin(), id.end(), 0);
        for (auto [u, v] : e) {
            int a = id[u], b = id[v]; assert(a != b);
            replace(id.begin() + 1, id.end(), b, a);
        }
        double x = rng.real(-4, 9); assert(-4 <= x && x < 9);
    }
    assert(Sampler::mix(1, 0) == 1082269761);
    BigInt a("123456789012345678901234567890");
    assert((a / 79) * 79 + a % 79 == a);
    Fraction x(2, 3), y(4, 7);
    assert((x + y) * (x - y) / y == Fraction(13, 63));
    for (int i = 0; i < 1000; i ++) {
        ull u = rng.rng(), v = rng.rng(), m = rng.rng() | 1;
        assert(wide_mul(u, v, m) == ull(BigInt(u) * v % m));
    }
    FILE *tmp = tmpfile(); assert(tmp);
    __int128_t lo = numeric_limits < __int128_t >::min();
    __uint128_t hi = numeric_limits < __uint128_t >::max();
    {
        FastOutput out(tmp); out.write(lo); out.put(' '); out.write(hi);
        out.write(" abc "); out.write(LLONG_MIN); out.put(' '); out.write(string(135000, 'q'));
    }
    rewind(tmp);
    FastInput in(tmp); __int128_t v; __uint128_t u; string word; ll l;
    assert(in.read(v) && v == lo && in.read(u) && u == hi && in.read(word) && word == "abc");
    assert(in.read(l) && l == LLONG_MIN && in.read(word) && word == string(135000, 'q'));
    assert(!in.read(l)); fclose(tmp);
    SparseTable < int, gcd < int, int > > st;
    st.build({0, 12, 18, 21}); assert(st.query(3, 1) == 3);
    for (int t = 0; t < 100; t ++) {
        V < ll > v{rng.integer(0, 80), rng.integer(0, 80)};
        for (bool up : {false, true}) {
            int next = 1;
            auto key = [&](ll i) { return make_pair((v[0] + up * (i - 1)) / i,
                                                    (v[1] + up * (i - 1)) / i); };
            quotient_blocks(v, 99, up, [&](ll l, ll r) {
                assert(l == next && key(l) == key(r) && (r == 99 || key(r) != key(r + 1)));
                next = r + 1;
            });
            assert(next == 100);
        }
    }
    using H = IntHull;
    using P = H::P;
    for (int t = 0; t < 2000; t ++) {
        V < P > a, b, pairs;
        for (auto *v : {&a, &b}) {
            int n = rng.integer(0, 9);
            for (int i = 0; i < n; i ++) v->emplace_back(rng.integer(-3, 3), rng.integer(-3, 3));
        }
        for (P p : a) for (P q : b) pairs.push_back(H::add(p, q));
        assert(H::sum(H::hull(a), H::hull(b)) == H::hull(pairs));
        for (bool upper : {false, true}) {
            auto h = H::chain(a, upper);
            for (size_t i = 1; i < h.size(); i ++) {
                assert(h[i - 1].fi < h[i].fi);
                for (P p : a) {
                    auto d = H::det(H::sub(h[i], h[i - 1]), H::sub(p, h[i - 1]));
                    assert(upper ? d <= 0 : d >= 0);
                }
            }
        }
    }
    ll k = 1000000000000000000ll;
    assert(H::det({k, k}, {-k, k}) == H::Int(2) * k * k);
}
