/*
静态区间幂等结合运算，数组和闭区间均为 1-based。ST 默认查最大值。
建表 O(n log n)，每次查询 O(1)。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

ll st_max(ll x, ll y) { return max(x, y); }

template < class T, auto op >
struct SparseTable {
    V < V < T > > st;
    void build(const V < T > &a) {
        int n = int(a.size()) - 1;
        int levels = n > 0 ? __lg(n) + 1 : 1;
        st.resize(levels);
        st[0] = a;
        for (int k = 1; k < levels; k ++) {
            int half = 1 << (k - 1);
            st[k].resize(n - 2 * half + 2);
            for (int i = 1; i < int(st[k].size()); i ++) {
                st[k][i] = op(st[k - 1][i], st[k - 1][i + half]);
            }
        }
    }
    T query(int l, int r) {
        if (l > r) swap(l, r);
        int k = __lg(r - l + 1);
        return op(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

using ST = SparseTable < ll, st_max >;

/*
ST st;
st.build(a);
ll ans = st.query(l, r);
SparseTable < int, gcd < int, int > > table;
table.build({0, 12, 18}); int d = table.query(1, 2);
*/
