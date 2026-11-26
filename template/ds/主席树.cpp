/*
静态数组 a[1..n] 的区间第 k 小、区间计数。要求 1 <= k <= r - l + 1。
建树 O(n log n)，查询 O(log n)。
count(l, r, x) 统计区间内 <= x 的元素个数。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct PST {
    struct Node {
        int ls = 0, rs = 0, sum = 0;
    };
    std::vector<Node> tr;
    std::vector<int> root;
    std::vector<i64> val;

    int insert(int p, int l, int r, int x) {
        int u = int(tr.size());
        tr.push_back(tr[p]);
        tr[u].sum ++;
        if (l == r) return u;
        int mid = (l + r) / 2;
        if (x <= mid)
            tr[u].ls = insert(tr[p].ls, l, mid, x);
        else
            tr[u].rs = insert(tr[p].rs, mid + 1, r, x);
        return u;
    }

    void build(const std::vector<i64> &a) {
        if (&a == &val) {
            std::vector<i64> b = a;
            build(b);
            return;
        }
        int n = int(a.size()) - 1;
        val.assign(a.begin() + 1, a.end());
        std::sort(val.begin(), val.end());
        val.erase(std::unique(val.begin(), val.end()), val.end());
        tr.assign(1, {});
        root.assign(n + 1, 0);
        for (int i = 1; i <= n; i ++) {
            int p = int(std::lower_bound(val.begin(), val.end(), a[i]) - val.begin());
            root[i] = insert(root[i - 1], 0, int(val.size()) - 1, p);
        }
    }

    int kth(int u, int v, int l, int r, int k) {
        if (l == r) return l;
        int mid = (l + r) / 2;
        int cnt = tr[tr[v].ls].sum - tr[tr[u].ls].sum;
        if (k <= cnt) return kth(tr[u].ls, tr[v].ls, l, mid, k);
        return kth(tr[u].rs, tr[v].rs, mid + 1, r, k - cnt);
    }

    i64 kth(int l, int r, int k) {
        return val[kth(root[l - 1], root[r], 0, int(val.size()) - 1, k)];
    }

    int count(int u, int v, int l, int r, int x) {
        if (r <= x) return tr[v].sum - tr[u].sum;
        int mid = (l + r) / 2;
        int ans = count(tr[u].ls, tr[v].ls, l, mid, x);
        if (x > mid) ans += count(tr[u].rs, tr[v].rs, mid + 1, r, x);
        return ans;
    }

    int count(int l, int r, i64 x) {
        int p = int(std::upper_bound(val.begin(), val.end(), x) - val.begin()) - 1;
        if (p < 0) return 0;
        return count(root[l - 1], root[r], 0, int(val.size()) - 1, p);
    }
};

/*
PST tree; tree.build({0, 5, 2, 5});
i64 x = tree.kth(1, 3, 2);
int cnt = tree.count(1, 3, 2);
*/
