/*
借助 Boost 的定宽整数计算 (a*b) % m，适用全部 ull，m > 0。固定字长下 O(1)。
Wide 支持流式十进制读写；用绝对值不超过 2^127-1 的输入可与通常的有符号宽整数交换。
*/

#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

typedef unsigned long long ull;

using namespace std;

using Wide = boost::multiprecision::int128_t;

ull wide_mul(ull a, ull b, ull m) {
    using UWide = boost::multiprecision::uint128_t;
    return ull(UWide(a) * b % m);
}

/*
Wide x; cin >> x; cout << x;
ull r = wide_mul(12345678901234567890ull, 17, 10000000000000000003ull);
*/
