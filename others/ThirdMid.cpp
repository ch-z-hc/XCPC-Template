#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;
typedef long double ld;

const int mod = 1e9 + 7;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

const ld eps = 1e-6;

int main() {
    ld l = 0, r = 10000000;

    auto f = [&](ld X) -> ld {
        // return val;
    };

    while (r - l > eps) { 
        ld lmid = l + (r - l) / 3;
        ld rmid = r - (r - l) / 3;
        if (f(lmid) < f(rmid)) // Get min
            r = rmid;
        else l = lmid; 

        if (f(lmid) < f(rmid)) // Get max
            l = lmid;
        else r = rmid; 
    }
    return 0;
}