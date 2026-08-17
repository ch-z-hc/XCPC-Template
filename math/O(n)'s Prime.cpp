#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
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


const int N0 = 1e7 + 5;

int prime[N0], vis[N0], tot;

void sieve(int N) {
    for (int i = 2; i <= N; ++ i) {
        if (! vis[i]) {
            vis[i] = i;
            prime[++ tot] = i;
        }
        for (int j = 1; j <= tot; ++ j) {
            if (prime[j] > vis[i] || prime[j] * i > N)
                break;
            vis[prime[j] * i] = prime[j];
        }
    }
}

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n; cin >> n;
    sieve(n);
    for (int i = 1; i <= tot; ++ i)
        cout << prime[i] << ' ';
    return 0;
}