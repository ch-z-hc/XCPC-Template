#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 主席树：静态数组的区间第 k 小、区间 <= x 的元素个数。
// 输入 a[1..n]，区间闭区间；k 从 1 开始且必须 <= r - l + 1。
// 自动离散化，构建 O(n log n)，查询 O(log n)，空间 O(n log n)。

struct PST {
    struct Node { int ls = 0, rs = 0, sum = 0; };
    V < Node > tr;
    V < int > root;
    V < ll > val;

    int insert(int p, int l, int r, int x) {
        int u = int(tr.size());
        tr.push_back(tr[p]); tr[u].sum ++;
        if (l == r) return u;
        int mid = (l + r) >> 1;
        if (x <= mid) tr[u].ls = insert(tr[p].ls, l, mid, x);
        else tr[u].rs = insert(tr[p].rs, mid + 1, r, x);
        return u;
    }

    void build(const V < ll > &a) {
        if (&a == &val) {
            V < ll > b = a;
            build(b); return;
        }
        int n = int(a.size()) - 1;
        val.assign(a.begin() + 1, a.end());
        sort(val.begin(), val.end());
        val.erase(unique(val.begin(), val.end()), val.end());
        tr.assign(1, {}); root.assign(n + 1, 0);
        for (int i = 1; i <= n; ++ i) {
            int p = int(lower_bound(val.begin(), val.end(), a[i]) - val.begin());
            root[i] = insert(root[i - 1], 0, int(val.size()) - 1, p);
        }
    }

    int kth(int u, int v, int l, int r, int k) {
        if (l == r) return l;
        int mid = (l + r) >> 1, cnt = tr[tr[v].ls].sum - tr[tr[u].ls].sum;
        if (k <= cnt) return kth(tr[u].ls, tr[v].ls, l, mid, k);
        return kth(tr[u].rs, tr[v].rs, mid + 1, r, k - cnt);
    }

    ll kth(int l, int r, int k) {
        return val[kth(root[l - 1], root[r], 0, int(val.size()) - 1, k)];
    }

    int count(int u, int v, int l, int r, int x) {
        if (r <= x) return tr[v].sum - tr[u].sum;
        int mid = (l + r) >> 1;
        int ans = count(tr[u].ls, tr[v].ls, l, mid, x);
        if (x > mid) ans += count(tr[u].rs, tr[v].rs, mid + 1, r, x);
        return ans;
    }

    int count(int l, int r, ll x) {
        int p = int(upper_bound(val.begin(), val.end(), x) - val.begin()) - 1;
        if (p < 0) return 0;
        return count(root[l - 1], root[r], 0, int(val.size()) - 1, p);
    }
};

// 用法：PST tree; tree.build(a); ll x = tree.kth(l, r, k);
