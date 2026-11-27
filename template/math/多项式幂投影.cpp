/*
先复制「多项式基本运算」。Power Projection 同时求 [x^(n-1)] A(x) B(x)^i，0 <= i < n。
要求 B(0) = 0，系数已按 998244353 取模。对 A/(1-yB) 按 x 二分取系数，
将二元卷积打包成一元 NTT。1 <= n <= 2^18，时间 O(n log² n)，空间 O(n)。
*/

#include <bits/stdc++.h>

#define V vector
using namespace std;

V < int > power_projection(V < int > a, V < int > b, int n) {
    int d = n - 1, mod = FPS::mod;
    a.resize(n); b.resize(n);
    assert(!b[0]);
    V < V < int > > p{a}, q(2, V < int >(n));
    q[0][0] = 1;
    for (int j = 0; j < n; j ++) q[1][j] = b[j] ? mod - b[j] : 0;
    while (d) {
        int stride = 2 * d + 1;
        auto pack = [&](const V < V < int > > &rows, bool neg) {
            V < int > f((rows.size() - 1) * stride + d + 1);
            for (int i = 0; i < int(rows.size()); i ++)
                for (int j = 0; j <= d; j ++) {
                    int x = rows[i][j];
                    f[i * stride + j] = neg && (j & 1) && x ? mod - x : x;
                }
            return f;
        };
        auto minus = pack(q, true);
        auto halve = [&](const V < V < int > > &rows, int parity) {
            auto h = FPS::mul(pack(rows, false), minus);
            int m = min(n, int(rows.size() + q.size() - 1));
            V < V < int > > out(m, V < int >(d / 2 + 1));
            for (int i = 0; i < m; i ++)
                for (int j = 0; j <= d / 2; j ++)
                    out[i][j] = h[i * stride + j * 2 + parity];
            return out;
        };
        p = halve(p, d & 1);
        q = halve(q, 0);
        d /= 2;
    }
    V < int > ans(n);
    for (int i = 0; i < int(p.size()); i ++) ans[i] = p[i][0];
    return ans;
}

/*
auto v = power_projection({1, 2, 3}, {0, 1}, 3);
*/
