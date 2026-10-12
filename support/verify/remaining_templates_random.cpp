#include <cassert>
#include "../../template/math/最短线性递推.cpp"
#include "../../template/math/拉格朗日插值.cpp"
#include "../../template/math/矩阵快速幂.cpp"
#include "../../template/math/快速沃尔什变换.cpp"
#include "../../template/math/快速数论变换与多项式乘法.cpp"
#include "../../template/math/组合数.cpp"
#include "../../template/math/线性筛积性函数.cpp"
#include "../../template/math/线性筛素数.cpp"
#include "../../template/math/异或线性基.cpp"
#include "../../template/math/整除分块.cpp"
#include "../../template/math/自适应辛普森积分.cpp"
#include "../../template/others/常用定义.cpp"
#include "../../template/others/离散化.cpp"
#include "../../template/others/树哈希.cpp"
#include "../../template/others/笛卡尔树.cpp"
#include "../../template/others/莫队.cpp"
#include "../../template/others/滑动窗口最值.cpp"
#include "../../template/others/稀疏表.cpp"
#include "../../template/others/三分搜索.cpp"
#include "../../template/string/多模式匹配自动机.cpp"
#include "../../template/string/后缀自动机.cpp"
#include "../../template/string/字典树.cpp"

V < ll > mo_array, mo_answer;
ll mo_sum = 0;
void Mo::add(int p) { mo_sum += mo_array[p]; }
void Mo::del(int p) { mo_sum -= mo_array[p]; }
void Mo::answer(int id) { mo_answer[id] = mo_sum; }

int main() {
    mt19937 rng(19260817);
    const ll mod = 998244353;
    for (int test = 0; test < 300; ++ test) {
        int d = int(rng() % 6) + 1;
        V < ll > rec(d), init(d), seq(60);
        for (ll &x : rec) x = rng() % mod;
        for (int i = 0; i < d; ++ i) seq[i] = init[i] = rng() % mod;
        for (int i = d; i < 60; ++ i)
            for (int j = 0; j < d; ++ j)
                seq[i] = (seq[i] + rec[j] * seq[i - j - 1]) % mod;
        auto found = bm(V < ll > (seq.begin(), seq.begin() + 2 * d + 2), mod);
        for (int i = 0; i < 60; ++ i) {
            assert(kth(init, rec, i, mod) == seq[i]);
            assert(kth(V < ll > (seq.begin(), seq.begin() + found.size()), found, i, mod) == seq[i]);
        }
        int n = 1 << (rng() % 5);
        V < ll > a(n), b(n), c;
        for (ll &x : a) x = rng() % mod;
        for (ll &x : b) x = rng() % mod;
        for (int op = 0; op < 3; ++ op) {
            V < ll > x = a, y = b, expected(n);
            for (int i = 0; i < n; ++ i) for (int j = 0; j < n; ++ j) {
                int k = op == 0 ? i | j : op == 1 ? i & j : i ^ j;
                expected[k] = (expected[k] + a[i] * b[j]) % mod;
            }
            if (op == 0) { fwt_or(x, mod); fwt_or(y, mod); }
            if (op == 1) { fwt_and(x, mod); fwt_and(y, mod); }
            if (op == 2) { fwt_xor(x, mod); fwt_xor(y, mod); }
            for (int i = 0; i < n; ++ i) x[i] = x[i] * y[i] % mod;
            if (op == 0) ifwt_or(x, mod);
            if (op == 1) ifwt_and(x, mod);
            if (op == 2) fwt_xor(x, mod, 1);
            assert(x == expected);
        }
        c.assign(2 * n - 1, 0);
        for (int i = 0; i < n; ++ i) for (int j = 0; j < n; ++ j)
            c[i + j] = (c[i + j] + a[i] * b[j]) % mod;
        assert(multiply(a, b) == c);
        V < ll > coef(d), x(d), y(d);
        for (ll &v : coef) v = rng() % mod;
        auto eval = [&](ll at) {
            ll ans = 0;
            for (int i = d - 1; i >= 0; -- i) ans = (ans * at + coef[i]) % mod;
            return ans;
        };
        for (int i = 0; i < d; ++ i) { x[i] = i * 3 + 1; y[i] = eval(x[i]); }
        ll at = rng() % mod;
        assert(lagrange(x, y, at, mod) == eval(at));

        Basis base;
        set < unsigned long long > values{0};
        for (int i = 0; i < 8; ++ i) {
            unsigned long long v = rng() % 256;
            base.insert(v);
            auto old = values;
            for (auto u : old) values.insert(u ^ v);
        }
        assert(base.max_xor() == *values.rbegin());
        for (int i = 0; i < 256; ++ i) assert(base.contains(i) == (values.count(i) != 0));
        base.clear(); assert(base.max_xor() == 0);

        n = int(rng() % 30) + 1;
        V < ll > arr(n + 1);
        for (int i = 1; i <= n; ++ i) arr[i] = int(rng() % 101) - 50;
        ST st; st.build(arr); st.build(arr);
        Cartesian tree; tree.build(arr);
        V < int > ord;
        auto dfs = [&](auto &&self, int u) -> void {
            if (! u) return;
            self(self, tree.ls[u]); ord.push_back(u); self(self, tree.rs[u]);
            for (int v : {tree.ls[u], tree.rs[u]}) if (v) {
                assert(tree.fa[v] == u && arr[u] <= arr[v]);
            }
        };
        dfs(dfs, tree.root);
        for (int i = 0; i < n; ++ i) assert(ord[i] == i + 1);
        for (int l = 1; l <= n; ++ l) for (int r = l; r <= n; ++ r)
            assert(st.query(l, r) == *max_element(arr.begin() + l, arr.begin() + r + 1));
        V < ll > data(arr.begin() + 1, arr.end());
        int k = int(rng() % n) + 1;
        auto low = window_min(data, k), high = window_max(data, k);
        for (int i = 0; i + k <= n; ++ i) {
            assert(low[i] == *min_element(data.begin() + i, data.begin() + i + k));
            assert(high[i] == *max_element(data.begin() + i, data.begin() + i + k));
        }
        V < Mo::Query > qs;
        V < ll > expected;
        for (int i = 0; i < 50; ++ i) {
            int l = int(rng() % n) + 1, r = int(rng() % n) + 1;
            if (l > r) swap(l, r);
            qs.push_back({l, r, i});
            expected.push_back(accumulate(arr.begin() + l, arr.begin() + r + 1, 0LL));
        }
        Mo mo;
        for (int repeat = 0; repeat < 2; ++ repeat) {
            mo_array = arr; mo_sum = 0; mo_answer.assign(qs.size(), 0);
            mo.solve(n, qs);
            assert(mo_answer == expected);
        }

        string s(rng() % 20, 'a');
        for (char &ch : s) ch = char('a' + rng() % 3);
        SAM sam; sam.build(s);
        map < string, ll > counts;
        for (int l = 0; l < int(s.size()); ++ l)
            for (int len = 1; l + len <= int(s.size()); ++ len) counts[s.substr(l, len)] ++;
        ll best = 0;
        auto cnt = sam.count();
        assert(cnt == sam.count());
        for (auto [t, num] : counts) {
            int u = 0;
            for (char ch : t) u = sam.tr[u].ch[ch - 'a'];
            assert(cnt[u] == num);
            if (num > 1) best = max(best, num * ll(t.size()));
        }
        assert(sam.query() == best);
        sam.build(""); assert(sam.tr.size() == 1);
        AC ac;
        Trie trie;
        V < string > patterns;
        V < ll > expected_counts;
        for (int i = 0; i < 10; ++ i) {
            string t(rng() % 5 + 1, 'a');
            for (char &ch : t) ch = char('a' + rng() % 3);
            patterns.push_back(t); ac.insert(t); trie.insert(t);
            ll num = 0;
            for (int j = 0; j + int(t.size()) <= int(s.size()); ++ j)
                if (s.compare(j, t.size(), t) == 0) num ++;
            expected_counts.push_back(num);
            ac.build(); ac.build();
            assert(ac.query(s) == expected_counts);
        }
        for (const string &t : patterns) {
            int words = 0, prefixes = 0;
            for (const string &p : patterns) {
                words += p == t;
                prefixes += p.size() >= t.size() && p.compare(0, t.size(), t) == 0;
            }
            assert(trie.count(t) == words && trie.prefix(t) == prefixes);
        }
        trie.insert(""); assert(trie.count("") == 1 && trie.prefix("") == 11);
        trie.clear(); ac.clear(); assert(trie.count("a") == 0 && ac.pos.empty());
    }
    Sieve sieve; Comb comb;
    for (int n : {0, 1, 2, 10, 200}) {
        sieve.build(n);
        assert(sieve.prime == primes(n));
        for (int i = 2; i <= n; ++ i) {
            int p = 2;
            while (i % p) p ++;
            assert(sieve.lp[i] == p);
        }
        comb.init(n, mod);
        V < ll > row(n + 1); row[0] = 1;
        for (int i = 0; i <= n; ++ i) {
            for (int j = 0; j <= i; ++ j) assert(comb.choose(i, j) == row[j]);
            for (int j = i + 1; j > 0 && j <= n; -- j) row[j] = (row[j] + row[j - 1]) % mod;
        }
    }
    for (int n = 1; n <= 1000; ++ n) {
        ll next = 1;
        for (auto [l, r] : div_blocks(n)) {
            assert(l == next && n / l == n / r);
            for (ll i = l; i <= r; ++ i) assert(n / i == n / l);
            next = r + 1;
        }
        assert(next == n + 1);
    }
    Matrix a{{1, 2}, {3, 4}}, ans{{1, 0}, {0, 1}};
    for (int i = 0; i <= 20; ++ i) {
        assert(matrix_pow(a, i, mod) == ans);
        ans = matrix_mul(ans, a, mod);
    }
    assert(multiply({}, {1}).empty());
    assert(abs(simpson([](double x) { return x * x; }, 0, 1) - 1.0 / 3) < 1e-8);
    assert(abs(ternary_min([](ld x) { return (x - 3) * (x - 3); }, -10, 10) - 3) < 1e-6L);
    assert(abs(ternary_max([](ld x) { return -(x + 2) * (x + 2); }, -10, 10) + 2) < 1e-6L);
    int x = 8; add(x, 5, 11); assert(x == 2); sub(x, 5, 11); assert(x == 8);
    assert(chkmin(x, 3) && x == 3); assert(chkmax(x, 7) && x == 7);
    map < string, uint64_t > shape_hash;
    map < uint64_t, string > hash_shape;
    for (int test = 0; test < 2000; ++ test) {
        V < ll > data(rng() % 51);
        for (ll &v : data) v = int(rng() % 21) - 10;
        if (test % 3 == 0) {
            data.push_back(LLONG_MIN); data.push_back(LLONG_MAX);
        }
        auto old = data;
        V < ll > values;
        auto id = discretize(data, values);
        set < ll > key(data.begin(), data.end());
        assert(values == V < ll > (key.begin(), key.end()));
        assert(data == old && id == discretize(data, values));
        for (int i = 0; i < int(data.size()); ++ i) {
            assert(id[i] == int(distance(key.begin(), key.find(data[i]))) + 1);
            assert(values[id[i] - 1] == data[i]);
        }

        int n = int(rng() % 30) + 1, root = int(rng() % n) + 1;
        V < V < int > > g(n + 1), h(n + 1);
        V < int > perm(n + 1);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm.begin() + 1, perm.end(), rng);
        for (int u = 2; u <= n; ++ u) {
            int v = int(rng() % (u - 1)) + 1;
            g[u].push_back(v); g[v].push_back(u);
            h[perm[u]].push_back(perm[v]); h[perm[v]].push_back(perm[u]);
        }
        for (int u = 1; u <= n; ++ u) shuffle(h[u].begin(), h[u].end(), rng);
        auto canonical = [&](auto &&self, int u, int fa) -> string {
            V < string > children;
            for (int v : g[u]) if (v != fa) children.push_back(self(self, v, u));
            sort(children.begin(), children.end());
            string s = "(";
            for (const string &child : children) s += child;
            return s + ")";
        };
        string shape = canonical(canonical, root, 0);
        uint64_t value = tree_hash(g, root);
        assert(value == tree_hash(h, perm[root]));
        if (shape_hash.count(shape)) assert(shape_hash[shape] == value);
        if (hash_shape.count(value)) assert(hash_shape[value] == shape);
        shape_hash[shape] = value; hash_shape[value] = shape;
    }
    V < ll > values{1, 2};
    assert(discretize(V < ll > {}, values).empty() && values.empty());
    return 0;
}
