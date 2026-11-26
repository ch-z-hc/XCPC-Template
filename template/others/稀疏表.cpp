#include <bits/stdc++.h>

using i64 = long long;

struct ST {
    std::vector<std::vector<i64>> st;
    void build(const std::vector<i64> &a) {
        int n = int(a.size()) - 1;
        int levels = n > 0 ? std::__lg(n) + 1 : 1;
        st.resize(levels);
        st[0] = a;
        for (int k = 1; k < levels; k ++) {
            int half = 1 << (k - 1);
            st[k].resize(n - 2 * half + 2);
            for (int i = 1; i < int(st[k].size()); i ++) {
                st[k][i] = std::max(st[k - 1][i], st[k - 1][i + half]);
            }
        }
    }
    i64 query(int l, int r) {
        if (l > r) std::swap(l, r);
        int k = std::__lg(r - l + 1);
        return std::max(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

/*
ST st;
st.build(a);
i64 ans = st.query(l, r);
*/
