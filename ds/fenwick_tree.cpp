#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree：单点加、前缀和与区间和；数组下标 1-based。
// 初始化 O(n)，更新和查询 O(log n)，空间 O(n)。
struct FenwickTree {
    int n = 0;
    vector<long long> tree;

    explicit FenwickTree(int n = 0) { init(n); }
    explicit FenwickTree(const vector<long long> &values) { build(values); }

    void init(int n_) {
        n = n_;
        tree.assign(n + 1, 0);
    }

    void build(const vector<long long> &values) {
        init(max(0, int(values.size()) - 1));
        for (int i = 1; i <= n; ++i) {
            tree[i] += values[i];
            int parent = i + (i & -i);
            if (parent <= n) tree[parent] += tree[i];
        }
    }

    void add(int index, long long delta) {
        for (int i = index; i <= n; i += i & -i) tree[i] += delta;
    }

    long long prefix_sum(int index) const {
        long long result = 0;
        for (int i = index; i > 0; i -= i & -i) result += tree[i];
        return result;
    }

    long long range_sum(int left, int right) const {
        return left <= right ? prefix_sum(right) - prefix_sum(left - 1) : 0;
    }
};

// Usage:
// FenwickTree fenwick(values);
// fenwick.add(index, delta);
// long long sum = fenwick.range_sum(left, right);
