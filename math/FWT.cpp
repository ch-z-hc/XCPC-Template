#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

const int mod = 998244353;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

int main() {
    int n; cin >> n;

    auto quick_pow = [&](int a, int b) {
        int res = 1;
        for (; b; b >>= 1) {
            if (b & 1) res = 1ll * res * a % mod;
            a = 1ll * a * a % mod;
        }
        return res;
    };

    V < int > a((1 << n) + 1), a0((1 << n) + 1);
    V < int > b((1 << n) + 1), b0((1 << n) + 1);
    V < int > c((1 << n) + 1);

    for (int i = 0; i < (1 << n); ++ i) {
        cin >> a0[i]; a[i] = a0[i];
    }
    for (int i = 0; i < (1 << n); ++ i) {
        cin >> b0[i]; b[i] = b0[i];
    }


    auto FWT_OR = [&](V < int > &A) {
        for (int len = 2; len <= (1 << n); len *= 2)
            for (int x = 0; x < (1 << n); x += len)
                for (int i = x; i < x + len / 2; ++ i)
                    A[i + len / 2] = (A[i] + A[i + len / 2]) % mod;
    };

    auto IFWT_OR = [&](V < int > &A) {
        for (int len = (1 << n); len >= 2; len /= 2)
            for (int x = 0; x < (1 << n); x += len)
                for (int i = x; i < x + len / 2; ++ i)
                    A[i + len / 2] = (A[i + len / 2] - A[i]) % mod;
    };
    
    auto FWT_AND = [&](V < int > &A) {
        for (int len = 2; len <= (1 << n); len *= 2)
            for (int x = 0; x < (1 << n); x += len)
                for (int i = x; i < x + len / 2; ++ i)
                    A[i] = (A[i] + A[i + len / 2]) % mod;
    };
    
    auto IFWT_AND = [&](V < int > &A) {
        for (int len = (1 << n); len >= 2; len /= 2)
            for (int x = 0; x < (1 << n); x += len)
                for (int i = x; i < x + len / 2; ++ i)
                    A[i] = (A[i] - A[i + len / 2]) % mod;
    };
    
    auto FWT_XOR = [&](V < int > &A) {
        for (int len = 2; len <= (1 << n); len *= 2)
            for (int x = 0; x < (1 << n); x += len)
                for (int i = x; i < x + len / 2; ++ i) {
                    A[i] = (A[i] + A[i + len / 2]) % mod;
                    A[i + len / 2] = (A[i] - 2 * A[i + len / 2]) % mod;
                }
    };
    
    auto IFWT_XOR = [&](V < int > &A) {
        for (int len = (1 << n); len >= 2; len /= 2)
            for (int x = 0; x < (1 << n); x += len)
                for (int i = x; i < x + len / 2; ++ i) {
                    A[i + len / 2] = 1ll * (A[i] - A[i + len / 2]) * quick_pow(2, mod - 2) % mod;
                    A[i] = (A[i] - A[i + len / 2]) % mod;
                }
    };

    FWT_OR(a), FWT_OR(b);

    for (int i = 0; i < (1 << n); ++ i)
        c[i] = 1ll * a[i] * b[i] % mod;

    IFWT_OR(c);

    for (int i = 0; i < (1 << n); ++ i)
        cout << (c[i] + mod) % mod << ' ';
    cout << '\n';

    for (int i = 0; i < (1 << n); ++ i)
        a[i] = a0[i], b[i] = b0[i];
    FWT_AND(a), FWT_AND(b);

    for (int i = 0; i < (1 << n); ++ i)
        c[i] = 1ll * a[i] * b[i] % mod;

    IFWT_AND(c);

    for (int i = 0; i < (1 << n); ++ i)
        cout << (c[i] + mod) % mod << ' ';
    cout << '\n';

    for (int i = 0; i < (1 << n); ++ i)
        a[i] = a0[i], b[i] = b0[i];
    FWT_XOR(a), FWT_XOR(b);

    for (int i = 0; i < (1 << n); ++ i)
        c[i] = 1ll * a[i] * b[i] % mod;

    IFWT_XOR(c);

    for (int i = 0; i < (1 << n); ++ i)
        cout << (c[i] + mod) % mod << ' ';
    return 0;   
}