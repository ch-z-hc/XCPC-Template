#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 枚举下标对，O(n^2)。
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n; V < int > a(n + 1);
    for (int i = 1; i <= n; ++ i) cin >> a[i];
    ll ans = 0;
    for (int i = 1; i <= n; ++ i)
        for (int j = i + 1; j <= n; ++ j) ans += a[i] > a[j];
    cout << ans << '\n';
}
