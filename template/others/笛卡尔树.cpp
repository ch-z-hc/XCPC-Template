#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 建立小根笛卡尔树；数组和节点编号均为 1-based。
// build 复杂度 O(n)，空间 O(n)；相等值不会弹出较早节点。

struct Cartesian {
    int root = 0;
    V < int > ls, rs, fa;

    void build(const V < ll > &a) {
        int n = int(a.size()) - 1;
        ls.assign(n + 1, 0);
        rs.assign(n + 1, 0);
        fa.assign(n + 1, 0);
        V < int > stk;
        for (int i = 1; i <= n; ++ i) {
            int lst = 0;
            while (! stk.empty() && a[stk.back()] > a[i]) lst = stk.back(), stk.pop_back();
            if (! stk.empty()) rs[stk.back()] = i, fa[i] = stk.back();
            if (lst) ls[i] = lst, fa[lst] = i;
            stk.push_back(i);
        }
        root = stk.empty() ? 0 : stk.front();
    }
};

// 用法：Cartesian tree;
// tree.build(a); // a[0] 为占位，tree.root 为根。
