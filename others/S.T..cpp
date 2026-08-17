#include <bits/stdc++.h>
using namespace std;

// 静态区间最大值；预处理 O(n log n)，查询 O(1)，数组 1-based。
struct SparseTable {
    vector<vector<long long>> st;
    vector<int> lg;
    void build(const vector<long long> &a) {
        int n = int(a.size()) - 1, levels = n ? 32 - __builtin_clz(n) : 1;
        st.assign(levels, vector<long long>(n + 1)); lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
        for (int i = 1; i <= n; ++i) st[0][i] = a[i];
        for (int k = 1; k < levels; ++k)
            for (int i = 1; i + (1 << k) - 1 <= n; ++i)
                st[k][i] = max(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
    }
    long long query(int l, int r) const {
        if (l > r) swap(l, r);
        int k = lg[r - l + 1];
        return max(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
