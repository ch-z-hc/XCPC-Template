#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 数组逐项修改 / 求和，O(nq)。
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, q; cin >> n >> q;
    V < ll > a(n + 1);
    for (int i = 1; i <= n; ++ i) cin >> a[i];
    while (q --) {
        int op, l, r; cin >> op >> l >> r;
        if (! op) {
            ll v; cin >> v;
            for (int i = l; i <= r; ++ i) a[i] += v;
        } else {
            ll ans = 0;
            for (int i = l; i <= r; ++ i) ans += a[i];
            cout << ans << '\n';
        }
    }
}
