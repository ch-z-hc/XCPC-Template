#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// 对拍标准程序示例：以 O(n) 计算非空最大子段和。
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    if (! (cin >> n)) return 0;

    ll ans = numeric_limits < ll >::lowest(), sum = 0;
    for (int i = 1; i <= n; ++ i) {
        ll x; cin >> x;
        sum = max(x, sum + x);
        ans = max(ans, sum);
    }
    cout << ans << '\n';
    return 0;
}
