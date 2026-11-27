// 最值更新和模加减均为 O(1)。模加减的两项都须在 [0, mod) 内，中间结果须放入 T。

#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
#define pii pair < int, int >
typedef long long ll;
using namespace std;

template < class T > bool chkmin(T &a, const T &b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T &a, const T &b) { return a < b ? a = b, 1 : 0; }
template < class T > void add(T &x, T y, T mod) {
    x += y;
    if (x >= mod) x -= mod;
}
template < class T > void sub(T &x, T y, T mod) {
    x -= y;
    if (x < 0) x += mod;
}

/*
int lo = 5, hi = 5; bool lower = chkmin(lo, 3), higher = chkmax(hi, 7);
int x = 6; add(x, 3, 7); sub(x, 4, 7);
*/
