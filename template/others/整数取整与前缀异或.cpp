/*
有符号除法向下、向上取整，除数非零且商可用 ll 表示。前缀异或和取 [0, n]。
整数平方根向下或向上取整；对数要求 a >= 2、n >= 1。除法与异或 O(1)，开根与对数 O(log n)。
*/

#include <bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

ll floor_div(ll a, ll b) {
    ll q = a / b, r = a % b;
    return q - (r != 0 && (r < 0) != (b < 0));
}

ll ceil_div(ll a, ll b) {
    ll q = a / b, r = a % b;
    return q + (r != 0 && (r < 0) == (b < 0));
}

ull int_sqrt(ull n, bool up = false) {
    ull l = 0, r = 1ull << 32;
    while (l + 1 < r) {
        ull m = (l + r) / 2;
        if (m <= n / m) l = m;
        else r = m;
    }
    return l + (up && l * l != n);
}

int int_log(ull a, ull n, bool up = false) {
    ull v = 1;
    int k = 0;
    while (v <= n / a) v *= a, k ++;
    return k + (up && v != n);
}

ull prefix_xor(ull n) {
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        default: return 0;
    }
}

/*
ll l = floor_div(-5, 2), r = ceil_div(-5, 2);
ull a = int_sqrt(15), b = int_sqrt(15, true);
int c = int_log(2, 9), d = int_log(2, 9, true);
ull x = prefix_xor(10);
*/
