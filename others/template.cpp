#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

constexpr ll MOD = 1000000007LL;

// 公共类型别名和 O(1) 的 chmin/chmax；不涉及数组下标。

template <class T>
bool chmin(T &a, const T &b) { return b < a ? (a = b, true) : false; }
template <class T>
bool chmax(T &a, const T &b) { return a < b ? (a = b, true) : false; }

// 这是公共代码片段，不包含 main()；比赛时自行添加 solve() 和唯一的 main()。
