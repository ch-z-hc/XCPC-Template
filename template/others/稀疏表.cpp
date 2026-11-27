#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct ST {
    V < V < ll > > st;
    void build(const V < ll > &a) {
        int n = int(a.size()) - 1;
        int levels = n > 0 ? __lg(n) + 1 : 1;
        st.resize(levels);
        st[0] = a;
        for (int k = 1; k < levels; k ++) {
            int half = 1 << (k - 1);
            st[k].resize(n - 2 * half + 2);
            for (int i = 1; i < int(st[k].size()); i ++) {
                st[k][i] = max(st[k - 1][i], st[k - 1][i + half]);
            }
        }
    }
    ll query(int l, int r) {
        if (l > r) swap(l, r);
        int k = __lg(r - l + 1);
        return max(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

/*
ST st;
st.build(a);
ll ans = st.query(l, r);
*/
