#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

using namespace std;

// 常用宏与更新函数；单次操作复杂度 O(1)，不涉及数组下标。
// add、sub 要求 x、y 已归一化到 [0, mod)，mod 显式传入，计算不得溢出 T。

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; }
template < class T > void add(T &x, T y, T mod) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y, T mod) { x -= y; if (x < 0) x += mod; }

// 调用示例：
// chkmin(ans, val);
// add(x, y, mod);
