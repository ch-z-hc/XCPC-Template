#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

template < class T > bool ckmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool ckmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 

using namespace std;

void solve() {

    int n, m, top = 0;
    cin >> n >> m;

    V < int > a(n + 1), sta(n + 1), ls(n + 1), rs(n + 1), fa(n + 1), dep(n + 1), sg(n + 1);

    for (int i = 1; i <= n; ++ i)
        cin >> a[i];

    for (int i = 1; i <= n; ++ i) {
        int k = top;
        while (k > 0 && a[sta[k]] > a[i])
            k --;
        if (k) rs[sta[k]] = i;
        if (k < top) ls[i] = sta[k + 1]; 
        sta[++ k] = i;
        top = k;
    }

    for (int i = 1; i <= n; ++ i) {
        if (ls[i]) fa[ls[i]] = i;
        if (rs[i]) fa[rs[i]] = i;
    }

    int rt = sta[1];

    int f[3][3] = {
        {1, 2, 1},
        {2, 2, 0},
        {1, 0, 1}
    };

    V < int > ord;
    sta.clear();
    sta.push_back(rt);

    while (!sta.empty()) {
        int u = sta.back();
        sta.pop_back();

        ord.push_back(u);

        if (ls[u]) {
            dep[ls[u]] = dep[u] + 1;
            sta.push_back(ls[u]);
        }

        if (rs[u]) {
            dep[rs[u]] = dep[u] + 1;
            sta.push_back(rs[u]);
        }
    }

    for (int i = n - 1; i >= 0; -- i) {
        int u = ord[i];
        sg[u] = f[sg[ls[u]]][sg[rs[u]]];
    }

    int LOG = __lg(n) + 2;

    V < V < int > > up(LOG, V < int > (n + 1));
    V < V < array < int, 3 > > > pre(LOG, V < array < int, 3 > > (n + 1));
    V < V < array < int, 3 > > > suf(LOG, V < array < int, 3 > > (n + 1));

    for (int k = 0; k < LOG; ++ k)
        for (int i = 0; i <= n; ++ i)
            pre[k][i] = suf[k][i] = {0, 1, 2};

    for (int u = 1; u <= n; ++ u) {
        int v = fa[u];

        up[0][u] = v;

        if (!v) continue;

        if (rs[v] == u)
            for (int s = 0; s < 3; ++ s)
                pre[0][u][s] = f[sg[ls[v]]][s];

        if (ls[v] == u)
            for (int s = 0; s < 3; ++ s)
                suf[0][u][s] = f[s][sg[rs[v]]];
    }

    for (int k = 1; k < LOG; ++ k) {
        for (int u = 1; u <= n; ++ u) {
            int mid = up[k - 1][u];

            up[k][u] = up[k - 1][mid];

            for (int s = 0; s < 3; ++ s) {
                pre[k][u][s] =
                    pre[k - 1][mid][pre[k - 1][u][s]];

                suf[k][u][s] =
                    suf[k - 1][mid][suf[k - 1][u][s]];
            }
        }
    }

    auto LCA = [&](int u, int v) {
        if (dep[u] < dep[v])
            swap(u, v);

        int d = dep[u] - dep[v];

        for (int k = 0; k < LOG; ++ k)
            if (d >> k & 1)
                u = up[k][u];

        if (u == v)
            return u;

        for (int k = LOG - 1; k >= 0; -- k) {
            if (up[k][u] != up[k][v]) {
                u = up[k][u];
                v = up[k][v];
            }
        }

        return fa[u];
    };

    auto getpre = [&](int root, int u) {
        int res = f[sg[ls[u]]][0];

        int d = dep[u] - dep[root];

        for (int k = 0; k < LOG; ++ k) {
            if (d >> k & 1) {
                res = pre[k][u][res];
                u = up[k][u];
            }
        }

        return res;
    };

    auto getsuf = [&](int root, int u) {
        int res = f[0][sg[rs[u]]];

        int d = dep[u] - dep[root];

        for (int k = 0; k < LOG; ++ k) {
            if (d >> k & 1) {
                res = suf[k][u][res];
                u = up[k][u];
            }
        }

        return res;
    };

    auto SG = [&](int l, int r) {
        if (l > r)
            return 0;
        int x = LCA(l, r);
        int L = 0, R = 0;
        if (l < x)
            L = getsuf(ls[x], l);
        if (x < r)
            R = getpre(rs[x], r);
        return f[L][R];
    };

    int ans = 0;
    while (m --) {
        int l, r;
        cin >> l >> r;
        ans ^= SG(l, r);
    }

    if (ans) cout << "Chito\n";
    else cout << "Yuuri\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t; cin >> t;
    while (t --) solve();
    return 0;
}
