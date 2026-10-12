#include <bits/stdc++.h>

typedef long long ll;
typedef __int128_t i128;

using namespace std;

// 类欧几里得：sum(i = 0..n - 1) floor((a * i + b) / m)。
// n >= 0，m > 0；a、b 可以为负数，使用数学意义的下取整。
// 参数使用 ll，答案与所有中间运算须在 i128 范围内。
// 时间 O(log m)，额外空间 O(1)；不取模。

i128 floor_sum(ll n_, ll m_, ll a_, ll b_) {
    i128 n = n_, m = m_, a = a_, b = b_, ans = 0;
    if (! n) return 0;
    if (a < 0) {
        i128 q = (a - m + 1) / m;
        ans += n * (n - 1) / 2 * q;
        a -= q * m;
    }
    if (b < 0) {
        i128 q = (b - m + 1) / m;
        ans += n * q;
        b -= q * m;
    }
    while (1) {
        ans += n * (n - 1) / 2 * (a / m) + n * (b / m);
        a %= m; b %= m;
        i128 y = a * n + b;
        if (y < m) break;
        n = y / m; b = y % m;
        swap(a, m);
    }
    return ans;
}

// 调用示例：i128 ans = floor_sum(4, 10, 6, 3); // ans = 3。
// floor_sum(4, 10, -6, 3) = -4。
// 输出先复制开头 template 的 print128：print128(ans); cout << '\n';
