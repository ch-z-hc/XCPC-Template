#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

const int mod = 1e9 + 7;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; }
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

// 输出宽整数，不附加换行；负数和最小值均可用，需 GCC / Clang。
void print128(__int128_t x) {
    if (x < 0) cout << '-';
    char s[40]; int n = 0;
    do {
        int d = int(x % 10);
        if (d < 0) d = -d;
        s[n ++] = char('0' + d); x /= 10;
    } while (x);
    while (n) cout << s[-- n];
}

void solve() {

}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // cin >> t;
    while (t --) solve();
    return 0;
}
