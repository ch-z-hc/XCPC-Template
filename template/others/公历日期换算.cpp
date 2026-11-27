/*
公历日期与连续日号互转，0001-01-01 对应 0；沿用公历闰年规则。
输入合法日期，年份在 [1, 10^9]；逆变换的日号须在相同范围内。时间 O(1)。
*/

#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

ll day_number(int y, int m, int d) {
    y -= m <= 2;
    int t = m > 2 ? m - 3 : m + 9;
    return 365ll * y + y / 4 - y / 100 + y / 400 + (153 * t + 2) / 5 + d - 307;
}

array < int, 3 > civil_date(ll day) {
    ll z = day + 306, era = z / 146097;
    int t = z % 146097;
    int y = (t - t / 1460 + t / 36524 - t / 146096) / 365;
    int d = t - (365 * y + y / 4 - y / 100);
    int m = (5 * d + 2) / 153;
    int dom = d - (153 * m + 2) / 5 + 1;
    return {int(400 * era + y + (m >= 10)), m < 10 ? m + 3 : m - 9, dom};
}

/*
ll day = day_number(2000, 2, 29);
auto [y, m, d] = civil_date(day + 1);
*/
