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

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n, k; cin >> n >> k;
    V < int > a(n + 1);
    struct node { int val, id; };
    deque < node > Q;

    for (int i = 1; i <= n; ++ i) cin >> a[i];
    for (int i = 1; i <= n; ++ i) { // min
        while (Q.size() && Q.front().id < i - k + 1) Q.pop_front();
        while (Q.size() && Q.back().val >= a[i]) Q.pop_back();
        Q.push_back(node{a[i], i});
        if (i >= k) cout << Q.front().val << ' ';
    }
    cout << '\n'; Q.clear();
    for (int i = 1; i <= n; ++ i) { // max
        while (Q.size() && Q.front().id < i - k + 1) Q.pop_front();
        while (Q.size() && Q.back().val <= a[i]) Q.pop_back();
        Q.push_back(node{a[i], i});
        if (i >= k) cout << Q.front().val << ' ';
    }
    return 0;
} 