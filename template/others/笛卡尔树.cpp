// 由 a[1..n] 建小根笛卡尔树，O(n)。以原数组下标编号，相等时较小下标更靠近根。

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct Cartesian {
    int root = 0;
    V < int > ls, rs, fa;

    void build(const V < ll > &a) {
        int n = int(a.size()) - 1;
        ls.assign(n + 1, 0);
        rs.assign(n + 1, 0);
        fa.assign(n + 1, 0);
        V < int > stk(n + 1);
        int top = 0;
        for (int i = 1; i <= n; i ++) {
            int k = top;
            while (k && a[stk[k]] > a[i]) k --;
            if (k) {
                rs[stk[k]] = i;
                fa[i] = stk[k];
            }
            if (k < top) {
                ls[i] = stk[k + 1];
                fa[ls[i]] = i;
            }
            top = k + 1;
            stk[top] = i;
        }
        root = top ? stk[1] : 0;
    }
};

/*
Cartesian tree;
tree.build(a);
*/
