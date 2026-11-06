#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 整体二分：带单点修改的区间第 k 小。数组 1-based，查询为闭区间。
// init(a) 后按时间顺序记录 modify / query，再 solve() 离线求解。
// query 返回从 0 开始的答案编号；k 必须在 [1, r - l + 1] 内。
// 时间 O((n + q) log V log n)，空间 O(n + q)，V 为出现过的不同值数。
// solve 不消耗记录，允许重复求解；init 清空之前全部记录。

struct KthOffline {
    struct Event {
        int type, l, r, k, delta, id, p = 0;
        ll val;
        bool left = false;
    };

    int n = 0, cnt = 0;
    V < ll > a, val, ans;
    V < Event > log;
    V < int > bit;

    void init(const V < ll > &input) {
        a = input; n = int(a.size()) - 1; cnt = 0;
        log.clear(); val.clear(); ans.clear();
        for (int i = 1; i <= n; ++ i) {
            log.push_back({0, i, 0, 0, 1, 0, 0, a[i]});
            val.push_back(a[i]);
        }
    }

    void modify(int p, ll x) {
        log.push_back({0, p, 0, 0, -1, 0, 0, a[p]});
        log.push_back({0, p, 0, 0, 1, 0, 0, x});
        a[p] = x; val.push_back(x);
    }

    int query(int l, int r, int k) {
        log.push_back({1, l, r, k, 0, cnt, 0, 0});
        return cnt ++;
    }

    void add(int p, int x) {
        for (; p <= n; p += p & -p) bit[p] += x;
    }

    int sum(int p) {
        int ans = 0;
        for (; p; p -= p & -p) ans += bit[p];
        return ans;
    }

    void solve(V < Event > &ev, V < Event > &tmp, int l, int r, int lo, int hi) {
        if (l > r) return;
        if (lo == hi) {
            for (int i = l; i <= r; ++ i) if (ev[i].type) ans[ev[i].id] = val[lo];
            return;
        }
        int mid = (lo + hi) >> 1, cnt = 0;
        for (int i = l; i <= r; ++ i) {
            auto &e = ev[i];
            if (! e.type) {
                e.left = e.p <= mid;
                if (e.left) add(e.l, e.delta);
            } else {
                int c = sum(e.r) - sum(e.l - 1);
                e.left = e.k <= c;
                if (! e.left) e.k -= c;
            }
            cnt += e.left;
        }
        int p = l, q = l + cnt;
        for (int i = l; i <= r; ++ i) {
            auto &e = ev[i];
            if (! e.type && e.left) add(e.l, -e.delta);
            if (e.left) tmp[p ++] = e;
            else tmp[q ++] = e;
        }
        for (int i = l; i <= r; ++ i) ev[i] = tmp[i];
        solve(ev, tmp, l, l + cnt - 1, lo, mid);
        solve(ev, tmp, l + cnt, r, mid + 1, hi);
    }

    V < ll > solve() {
        ans.assign(cnt, 0);
        if (! cnt) return ans;
        sort(val.begin(), val.end());
        val.erase(unique(val.begin(), val.end()), val.end());
        V < Event > ev = log, tmp(ev.size());
        for (auto &e : ev) if (! e.type)
            e.p = int(lower_bound(val.begin(), val.end(), e.val) - val.begin());
        bit.assign(n + 1, 0);
        solve(ev, tmp, 0, int(ev.size()) - 1, 0, int(val.size()) - 1);
        return ans;
    }
};

// 用法：KthOffline solver; solver.init(a); solver.modify(p, x);
// int id = solver.query(l, r, k); auto ans = solver.solve(); // 答案为 ans[id]。
