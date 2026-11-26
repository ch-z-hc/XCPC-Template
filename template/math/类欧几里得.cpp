/*
求 i 从 0 到 n - 1 的 floor((a*i + b) / m) 之和，O(log m)。n >= 0，m > 0，a、b 可负。
a*n+b、负数归一化的部分和及答案均须放入 i64。
*/

#include <bits/stdc++.h>

using i64 = long long;

i64 floor_sum(i64 n, i64 m, i64 a, i64 b) {
    i64 ans = 0;
    if (!n) return 0;
    if (a < 0) {
        i64 q = a / m - (a % m < 0);
        i64 tri = n % 2 ? n * ((n - 1) / 2) : (n / 2) * (n - 1);
        ans += tri * q;
        a %= m;
        if (a < 0) a += m;
    }
    if (b < 0) {
        i64 q = b / m - (b % m < 0);
        ans += n * q;
        b %= m;
        if (b < 0) b += m;
    }
    while (1) {
        if (a >= m) {
            i64 tri = n % 2 ? n * ((n - 1) / 2) : (n / 2) * (n - 1);
            ans += tri * (a / m);
        }
        ans += n * (b / m);
        a %= m;
        b %= m;
        i64 y = a * n + b;
        if (y < m) break;
        n = y / m;
        b = y % m;
        std::swap(a, m);
    }
    return ans;
}

// i64 ans = floor_sum(4, 10, -6, 3);
