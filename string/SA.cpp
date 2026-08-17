#include <bits/stdc++.h>
using namespace std;

// 后缀数组、Height 和 LCP 查询；复杂度 O(n log n)，下标 0-based。
void build_sa(const string &s, vector<int> &sa, vector<int> &rk,
              vector<int> &height, vector<int> &lg,
              vector<vector<int>> &st) {
    int n = int(s.size()), m = 256;
    sa.resize(n);
    rk.resize(n);
    height.assign(n, 0);
    lg.assign(n + 1, 0);
    st.clear();
    if (!n) return;

    vector<int> oldrk(n), id(n), px(n), cnt(max(n, m));
    for (int i = 0; i < n; ++i) ++cnt[rk[i] = static_cast<unsigned char>(s[i])];
    for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i) sa[--cnt[rk[i]]] = i;

    for (int w = 1, p;; w <<= 1, m = p) {
        p = 0;
        for (int i = n - w; i < n; ++i) if (i >= 0) id[p++] = i;
        for (int i = 0; i < n; ++i) if (sa[i] >= w) id[p++] = sa[i] - w;

        fill(cnt.begin(), cnt.begin() + m, 0);
        for (int i = 0; i < n; ++i) ++cnt[px[i] = rk[id[i]]];
        for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--cnt[px[i]]] = id[i];

        oldrk = rk;
        p = 1;
        rk[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            int a = sa[i], b = sa[i - 1];
            bool same = oldrk[a] == oldrk[b];
            same = same && (a + w < n ? oldrk[a + w] : -1) ==
                           (b + w < n ? oldrk[b + w] : -1);
            rk[a] = same ? p - 1 : p++;
        }
        if (p == n) break;
    }

    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 0, k = 0; i < n; ++i) {
        if (!rk[i]) {
            k = 0;
            continue;
        }
        if (k) --k;
        int j = sa[rk[i] - 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
        height[rk[i]] = k;
    }

    st.assign(lg[n] + 1, vector<int>(n));
    st[0] = height;
    for (int j = 1; j <= lg[n]; ++j)
        for (int i = 0; i + (1 << j) <= n; ++i)
            st[j][i] = min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
}

int lcp_sa(int x, int y, const vector<int> &rk, const vector<int> &lg,
           const vector<vector<int>> &st, int n) {
    if (x == y) return n - x;
    int l = rk[x], r = rk[y];
    if (l > r) swap(l, r);
    ++l;
    int k = lg[r - l + 1];
    return min(st[k][l], st[k][r - (1 << k) + 1]);
}

// Usage:
// vector<int> sa, rk, height, lg;
// vector<vector<int>> st;
// build_sa(s, sa, rk, height, lg, st);
// int common = lcp_sa(x, y, rk, lg, st, int(s.size()));
