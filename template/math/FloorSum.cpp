#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

// 类欧几里得：sum(i = 0..n - 1) floor((a * i + b) / m)。
// n >= 0，m > 0；a、b 可以为负数，使用数学意义的下取整。
// 参数、答案和所有中间量均须在 ll 范围内，尤其是 a * n + b。
// 三角和先除后乘；负参数归一化产生的两部分答案也须分别放入 ll。
// 时间 O(log m)，额外空间 O(1)；不取模。

ll floor_sum(ll n, ll m, ll a, ll b) {
    ll ans = 0;
    if (! n) return 0;
    if (a < 0) {
        ll q = a / m - (a % m < 0);
        ll tri = n % 2 ? n * ((n - 1) / 2) : (n / 2) * (n - 1);
        ans += tri * q;
        a %= m;
        if (a < 0) a += m;
    }
    if (b < 0) {
        ll q = b / m - (b % m < 0);
        ans += n * q;
        b %= m;
        if (b < 0) b += m;
    }
    while (1) {
        if (a >= m) {
            ll tri = n % 2 ? n * ((n - 1) / 2) : (n / 2) * (n - 1);
            ans += tri * (a / m);
        }
        ans += n * (b / m);
        a %= m; b %= m;
        ll y = a * n + b;
        if (y < m) break;
        n = y / m; b = y % m;
        swap(a, m);
    }
    return ans;
}

// 调用示例：ll ans = floor_sum(4, 10, 6, 3); // ans = 3。
// floor_sum(4, 10, -6, 3) = -4。
// cout << ans << '\n';
