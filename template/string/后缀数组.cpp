#include <bits/stdc++.h>

#define V vector

using namespace std;

// 后缀数组；字符串下标 0-based。build 复杂度 O(n log n)，lcp 查询 O(1)，空间 O(n log n)。
// sa 按字典序保存后缀起点，rk[i] 是后缀 s[i..] 的排名。

struct SA {
    string s;
    V < int > sa, rk, height, lg;
    V < V < int > > st;

    SA() = default;
    SA(const string &s) { build(s); }

    void build(const string &str) {
        s = str;
        int n = int(s.size()), m = 256;
        sa.resize(n);
        rk.resize(n);
        height.assign(n, 0);
        lg.assign(n + 1, 0);
        st.clear();
        if (n == 0) return;

        V < int > old(n), y(n), x(n), cnt(max(n, m));
        for (int i = 0; i < n; ++ i)
            ++ cnt[rk[i] = (unsigned char)s[i]];
        for (int i = 1; i < m; ++ i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; -- i) sa[-- cnt[rk[i]]] = i;

        for (int k = 1, p;; k <<= 1, m = p) {
            p = 0;
            for (int i = n - k; i < n; ++ i)
                if (i >= 0) y[p ++] = i;
            for (int i = 0; i < n; ++ i)
                if (sa[i] >= k) y[p ++] = sa[i] - k;

            fill(cnt.begin(), cnt.begin() + m, 0);
            for (int i = 0; i < n; ++ i) ++ cnt[x[i] = rk[y[i]]];
            for (int i = 1; i < m; ++ i) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; -- i)
                sa[-- cnt[x[i]]] = y[i];

            old = rk;
            p = 1;
            rk[sa[0]] = 0;
            for (int i = 1; i < n; ++ i) {
                int u = sa[i], v = sa[i - 1];
                bool same = old[u] == old[v];
                same = same && (u + k < n ? old[u + k] : -1) ==
                               (v + k < n ? old[v + k] : -1);
                rk[u] = same ? p - 1 : p ++;
            }
            if (p == n) break;
        }

        for (int i = 2; i <= n; ++ i) lg[i] = lg[i >> 1] + 1;
        for (int i = 0, len = 0; i < n; ++ i) {
            if (rk[i] == 0) {
                len = 0;
                continue;
            }
            if (len) -- len;
            int v = sa[rk[i] - 1];
            while (i + len < n && v + len < n &&
                   s[i + len] == s[v + len])
                ++ len;
            height[rk[i]] = len;
        }

        st.assign(lg[n] + 1, V < int > (n));
        st[0] = height;
        for (int j = 1; j <= lg[n]; ++ j)
            for (int i = 0; i + (1 << j) <= n; ++ i)
                st[j][i] = min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
    }

    int lcp(int u, int v) {
        if (u == v) return int(s.size()) - u;
        int l = rk[u], r = rk[v];
        if (l > r) swap(l, r);
        ++ l;
        int j = lg[r - l + 1];
        return min(st[j][l], st[j][r - (1 << j) + 1]);
    }
};

// 用法：SA sa(s);
// int len = sa.lcp(x, y);
