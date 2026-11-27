/*
先复制「多项式基本运算」「多项式多点求值与快速插值」。单次求 n! 模 998244353，n >= 0。
令 b 为 sqrt(n) 下取整，将 (x+1)..(x+b) 的乘积在整块起点求值。
时间 O(b log²(b+1))，空间 O(b log(b+1))。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

int fast_factorial(ll n) {
    int mod = FPS::mod;
    if (n >= mod) return 0;
    if (n <= 1) return 1;
    int b = int(sqrt(n));
    V < int > x(b);
    for (int i = 0; i < b; i ++) x[i] = mod - i - 1;
    Multipoint blocks; blocks.build(x);
    auto f = blocks.prod[1];
    for (int i = 0; i < b; i ++) x[i] = i * b;
    blocks.build(x);
    ll ans = 1;
    for (int v : blocks.eval(f)) ans = ans * v % mod;
    for (ll i = b * b + 1; i <= n; i ++) ans = ans * i % mod;
    return int(ans);
}

/*
int x = fast_factorial(100000000);
*/
