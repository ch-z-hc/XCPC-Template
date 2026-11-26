/*
求 1..n 的欧拉函数和、莫比乌斯函数和，0 <= n <= 4×10^9，结果用 i64，多次查询共用缓存。
1 <= limit < INT_MAX。对最大查询 N 取 limit 约 N^(2/3)，耗时 O(N^(2/3))，空间 O(limit + N / limit)。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct DuJiao {
    int limit;
    std::vector<i64> sp, sm;
    std::unordered_map<i64, i64> mp;
    std::unordered_map<i64, i64> mm;

    void init(int n) {
        assert(1 <= n && n < INT_MAX);
        limit = n;
        sp.assign(n + 1, 0);
        sm.assign(n + 1, 0);
        mp.clear();
        mm.clear();
        std::vector<int> prime;
        sp[1] = sm[1] = 1;
        for (int i = 2; i <= n; i ++) {
            if (!sp[i]) {
                prime.push_back(i);
                sp[i] = i - 1;
                sm[i] = -1;
            }
            for (int p : prime) {
                if (1ll * i * p > n) break;
                int x = i * p;
                if (i % p == 0) {
                    sp[x] = sp[i] * p;
                    sm[x] = 0;
                    break;
                }
                sp[x] = sp[i] * (p - 1);
                sm[x] = -sm[i];
            }
        }
        for (int i = 1; i <= n; i ++) {
            sp[i] += sp[i - 1];
            sm[i] += sm[i - 1];
        }
    }

    i64 phi(i64 n) {
        assert(0 <= n && n <= 4000000000LL);
        if (n <= limit) return sp[n];
        auto it = mp.find(n);
        if (it != mp.end()) return it->second;
        i64 ans = n % 2 ? n * ((n + 1) / 2) : (n / 2) * (n + 1);
        for (i64 l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= (r - l + 1) * phi(n / l);
        }
        return mp[n] = ans;
    }

    i64 mu(i64 n) {
        assert(0 <= n && n <= 4000000000LL);
        if (n <= limit) return sm[n];
        auto it = mm.find(n);
        if (it != mm.end()) return it->second;
        i64 ans = 1;
        for (i64 l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= (r - l + 1) * mu(n / l);
        }
        return mm[n] = ans;
    }
};

/*
DuJiao d; d.init(5000000);
i64 phi_sum = d.phi(n), mu_sum = d.mu(n);
*/
