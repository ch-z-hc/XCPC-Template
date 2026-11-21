#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

using namespace std;

/*
chkmin、chkmax 更新成功时返回 true。add、sub 要求 x 和 y 已在 [0, mod) 内。
模数由调用时传入，加减的中间结果不能溢出 T。每次操作 O(1)。
*/

template < class T > bool chkmin(T &a, const T &b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T &a, const T &b) { return a < b ? a = b, 1 : 0; }
template < class T > void add(T &x, T y, T mod) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y, T mod) { x -= y; if (x < 0) x += mod; }

/*
chkmin(ans, val);
add(x, y, mod);
*/
