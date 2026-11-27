/*
求 1..n 的欧拉函数和、莫比乌斯函数和，0 <= n <= 4×10^9，结果用 ll，多次查询共用缓存。
1 <= limit < INT_MAX。对最大查询 N 取 limit 约 N^(2/3)，耗时 O(N^(2/3))，空间 O(limit + N / limit)。
*/

#include <bits/stdc++.h>

#define V vector
#define se second

typedef long long ll;

using namespace std;

struct DuJiao {
    int limit;
    V < ll > sp, sm;
    unordered_map < ll, ll > mp;
    unordered_map < ll, ll > mm;

    void init(int n) {
        assert(1 <= n && n < INT_MAX);
        limit = n;
        sp.assign(n + 1, 0);
        sm.assign(n + 1, 0);
        mp.clear();
        mm.clear();
        V < int > prime;
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

    ll phi(ll n) {
        assert(0 <= n && n <= 4000000000LL);
        if (n <= limit) return sp[n];
        auto it = mp.find(n);
        if (it != mp.end()) return it->se;
        ll ans = n % 2 ? n * ((n + 1) / 2) : (n / 2) * (n + 1);
        for (ll l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= (r - l + 1) * phi(n / l);
        }
        return mp[n] = ans;
    }

    ll mu(ll n) {
        assert(0 <= n && n <= 4000000000LL);
        if (n <= limit) return sm[n];
        auto it = mm.find(n);
        if (it != mm.end()) return it->se;
        ll ans = 1;
        for (ll l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= (r - l + 1) * mu(n / l);
        }
        return mm[n] = ans;
    }
};

/*
DuJiao d; d.init(5000000);
ll phi_sum = d.phi(n), mu_sum = d.mu(n);
*/
