#include <bits/stdc++.h>
using namespace std;

// 返回 n / l 取值相同的所有最大区间 [l, r]。
vector<pair<long long, long long>> quotient_blocks(long long n) {
    vector<pair<long long, long long>> blocks;
    for (long long l = 1; l <= n;) {
        long long r = n / (n / l);
        blocks.push_back({l, r});
        l = r + 1;
    }
    return blocks;
}
