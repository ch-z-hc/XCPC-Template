#include <bits/stdc++.h>
#include <cassert>

#define V vector

typedef long long ll;
typedef __int128_t i128;

using namespace std;

// 杜教筛：phi(n)、mu(n) 分别返回 1..n 的欧拉函数、莫比乌斯函数之和。
// 0 <= n <= 1e10；phi 的结果可能超过 ll，返回 i128。
// init(limit) 要求 1 <= limit < INT_MAX，且筛表能放入内存。
// 线性筛 O(limit)，同时清空记忆化缓存。
// 最大询问为 N 时，取 limit 约 N^(2/3)，总时间 O(N^(2/3))。
// 空间 O(limit + N / limit)；多次询问可以共享缓存。

struct DuJiao {
    int limit;
    V < ll > sp, sm;
    unordered_map < ll, i128 > mp;
    unordered_map < ll, ll > mm;

    void init(int n) {
        assert(1 <= n && n < INT_MAX);
        limit = n;
        sp.assign(n + 1, 0); sm.assign(n + 1, 0);
        mp.clear(); mm.clear();
        V < int > prime;
        sp[1] = sm[1] = 1;
        for (int i = 2; i <= n; ++ i) {
            if (! sp[i]) {
                prime.push_back(i);
                sp[i] = i - 1; sm[i] = -1;
            }
            for (int p : prime) {
                if (1ll * i * p > n) break;
                int x = i * p;
                if (i % p == 0) {
                    sp[x] = sp[i] * p; sm[x] = 0;
                    break;
                }
                sp[x] = sp[i] * (p - 1); sm[x] = -sm[i];
            }
        }
        for (int i = 1; i <= n; ++ i) {
            sp[i] += sp[i - 1]; sm[i] += sm[i - 1];
        }
    }

    i128 phi(ll n) {
        if (n <= limit) return sp[n];
        auto it = mp.find(n);
        if (it != mp.end()) return it->second;
        i128 ans = i128(n) * (n + 1) / 2;
        for (ll l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= i128(r - l + 1) * phi(n / l);
        }
        return mp[n] = ans;
    }

    ll mu(ll n) {
        if (n <= limit) return sm[n];
        auto it = mm.find(n);
        if (it != mm.end()) return it->second;
        ll ans = 1;
        for (ll l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= (r - l + 1) * mu(n / l);
        }
        return mm[n] = ans;
    }
};

// 调用示例：DuJiao d; d.init(5000000);
// i128 a = d.phi(n); ll b = d.mu(n);
// 输出先复制开头 template 的 print128：print128(a); cout << '\n';
