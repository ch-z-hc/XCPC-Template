#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 由 a[1..n] 建小根笛卡尔树，O(n)。以原数组下标编号，相等时较小下标更靠近根。

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

/*
Cartesian tree;
tree.build(a); // a[0] 为占位，tree.root 为根。
*/
