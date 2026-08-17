#include <bits/stdc++.h>
using namespace std;

// 建立小根笛卡尔树；数组和节点编号均为 1-based。
// build 复杂度 O(n)，空间 O(n)；相等值不会弹出较早节点。
struct CartesianTree {
    int root = 0;
    vector<int> left, right, parent;

    void build(const vector<long long> &value) {
        int n = int(value.size()) - 1;
        left.assign(n + 1, 0); right.assign(n + 1, 0); parent.assign(n + 1, 0);
        vector<int> stack;
        for (int i = 1; i <= n; ++i) {
            int last = 0;
            while (!stack.empty() && value[stack.back()] > value[i]) last = stack.back(), stack.pop_back();
            if (!stack.empty()) right[stack.back()] = i, parent[i] = stack.back();
            if (last) left[i] = last, parent[last] = i;
            stack.push_back(i);
        }
        root = stack.empty() ? 0 : stack.front();
    }
};
