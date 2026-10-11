#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 对拍暴力程序示例：以 O(n^2) 计算非空最大子段和。
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    if (! (cin >> n)) return 0;

    V < ll > a(n + 1);
    for (int i = 1; i <= n; ++ i) cin >> a[i];

    ll ans = numeric_limits < ll >::lowest();
    for (int l = 1; l <= n; ++ l) {
        ll sum = 0;
        for (int r = l; r <= n; ++ r) {
            sum += a[r];
            ans = max(ans, sum);
        }
    }
    cout << ans << '\n';
    return 0;
}
