#include <cassert>
#include <sstream>

#define main contest_entry_main
#include "../tools/contest_main.cpp"
#undef main

#include "../../template/ds/LiChaoTree.cpp"
#include "../../template/ds/SegTree.cpp"
#include "../../template/graph/HLD.cpp"
#include "../../template/math/Gauss.cpp"
#include "../../template/math/Min25.cpp"

int main(int argc, char **) {
    if (argc > 1) {
        LiChaoTree unsupported(INT_MIN, INT_MAX);
        return 0;
    }
    {
        HLD hld(3);
        hld.add_edge(1, 3); hld.add_edge(1, 2); hld.build();
        V < ll > a{0, 10, 20, 30}, b(4);
        for (int u = 1; u <= 3; ++ u) b[hld.in[u]] = a[u];
        SegTree seg; seg.init(b); ll ans = 0;
        hld.path(1, 3, [&](int l, int r) { ans += seg.query(l, r); });
        assert(ans == 40);
    }
    {
        LiChaoTree tree(1001, 1001);
        LiChaoInt ans = 0;
        assert(! tree.query_int(1001, ans));
        tree.add_line(1000000000000000000LL, 1);
        assert(tree.query_int(1001, ans));
        assert(ans == LiChaoInt(1000000000000000000LL) * 1001 + 1);
        tree.add_line(1000000000000000000LL, 2);
        assert(tree.query_int(1001, ans));
        assert(ans == LiChaoInt(1000000000000000000LL) * 1001 + 2);
        tree.clear();
        assert(! tree.query_int(1001, ans));
        tree.add_line(-1000000000000000000LL, -1);
        assert(tree.query_int(1001, ans));
        assert(ans == -LiChaoInt(1000000000000000000LL) * 1001 - 1);
        LiChaoTree real(1, 2);
        real.add_line_real(0.5L, 0.0L);
        assert(real.query(2) == 1 && ! real.query_int(2, ans));
        LiChaoTree segment(1, 5);
        segment.add_seg(1, 2, 5, 7);
        assert(! segment.query_int(3, ans) && segment.query(3) == 4.5L);
        for (int l : {INT_MIN, INT_MAX - 2}) {
            LiChaoTree endpoint(l, l + 2);
            endpoint.add_line(2, 3);
            for (int offset = 0; offset < 3; ++ offset) {
                int x = l + offset;
                assert(endpoint.query_int(x, ans) && ans == LiChaoInt(2) * x + 3);
            }
        }
    }
    {
        __int128_t hi = ((__int128_t(1) << 126) - 1) * 2 + 1;
        __int128_t lo = -hi - 1;
        ostringstream output;
        streambuf *saved = cout.rdbuf(output.rdbuf());
        for (__int128_t x : {__int128_t(0), __int128_t(123), __int128_t(-456), hi, lo}) {
            print128(x); cout << '\n';
        }
        cout.rdbuf(saved);
        assert(output.str() == "0\n123\n-456\n"
            "170141183460469231731687303715884105727\n"
            "-170141183460469231731687303715884105728\n");
    }
    {
        const int N = 1005; int n = 2, m = 2;
        V < bitset < N > > c(n);
        c[0][0] = c[0][1] = 1; c[0][m] = 1;
        c[1][1] = 1; c[1][m] = 0;
        V < int > z; int op = gauss_xor(c, m, z);
        assert(op == 1 && z == V < int > ({1, 0}));
    }
    {
        Min25 sieve;
        for (ll modulus : {2, 6, 1000000007}) {
            for (int n = 0; n <= 100; ++ n) {
                sieve.init(n, modulus);
                ll ans = sieve.sum({2, 0, 0}, [](ll, int e) -> ll { return e + 1; });
                ll expected = 0;
                for (int i = 1; i <= n; ++ i)
                    for (int d = 1; d <= i; ++ d) expected += i % d == 0;
                assert(ans == expected % modulus);
            }
        }
    }
    cout << "PASS: print usage, HLD order, exact Li Chao and int128 output\n";
}
