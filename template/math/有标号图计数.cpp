/*
先复制「组合数」「多项式基本运算」。返回 0..n 个有标号顶点的简单图计数，模 998244353。
有向图允许反向边共存。欧拉图按连通且各点度数为偶数计；二分图不指定染色。
n < 2^21，各函数时间 O(n log(n+2))，空间 O(n)。
*/

#include <bits/stdc++.h>

#define V vector
typedef long long ll;
using namespace std;

struct GraphCount {
    int n, mod = FPS::mod;
    Comb cb;
    V < int > tri, inv_tri;

    explicit GraphCount(int n) : n(n), tri(n + 1, 1), inv_tri(n + 1, 1) {
        cb.init(n, mod);
        ll p = 1, q = 1;
        for (int i = 1; i <= n; i ++) {
            tri[i] = tri[i - 1] * p % mod;
            inv_tri[i] = inv_tri[i - 1] * q % mod;
            p = p * 2 % mod;
            q = q * ((mod + 1) / 2) % mod;
        }
    }

    V < int > counts(V < int > a) {
        for (int i = 0; i <= n; i ++) a[i] = a[i] * cb.fac[i] % mod;
        return a;
    }
    V < int > connected(bool euler = false) {
        V < int > a(n + 1, 1);
        for (int i = 1; i <= n; i ++)
            a[i] = tri[i - int(euler)] * cb.ifac[i] % mod;
        return counts(FPS::ln(a, n + 1));
    }
    V < int > dag(bool weak = false) {
        V < int > a(n + 1);
        for (int i = 0; i <= n; i ++) {
            a[i] = inv_tri[i] * cb.ifac[i] % mod;
            if (i & 1) a[i] = mod - a[i];
        }
        a = FPS::inv(a, n + 1);
        for (int i = 0; i <= n; i ++) a[i] = 1ll * a[i] * tri[i] % mod;
        return counts(weak ? FPS::ln(a, n + 1) : a);
    }
    V < int > strong() {
        V < int > a(n + 1);
        for (int i = 0; i <= n; i ++) a[i] = tri[i] * cb.ifac[i] % mod;
        a = FPS::inv(a, n + 1);
        for (int i = 1; i <= n; i ++) a[i] = 1ll * a[i] * tri[i] % mod;
        a = FPS::ln(a, n + 1);
        for (int &x : a) if (x) x = mod - x;
        return counts(a);
    }
    V < int > bipartite() {
        V < int > a(n + 1);
        for (int i = 0; i <= n; i ++) a[i] = inv_tri[i] * cb.ifac[i] % mod;
        a = FPS::mul(a, a);
        a.resize(n + 1);
        for (int i = 0; i <= n; i ++) a[i] = 1ll * a[i] * tri[i] % mod;
        V < int > root;
        FPS::sqrt(a, n + 1, root);
        return counts(root);
    }
};

/*
GraphCount g(1000);
auto connected = g.connected(), euler = g.connected(true);
auto dag = g.dag(), weak = g.dag(true);
auto strong = g.strong(), bip = g.bipartite();
*/
