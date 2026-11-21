#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

/*
floor_sum(n, m, a, b) 累加 i = 0..n - 1 时的 floor((a*i + b) / m)。
n >= 0，m > 0，a、b 可为负，除法向下取整。a*n+b、负数归一化时的各部分和及答案须能放进 ll。
时间 O(log m)，额外空间 O(1)。
*/

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

// ll ans = floor_sum(4, 10, -6, 3); // -4
