/*
离线处理树上路径询问，增删按顶点。先建树再记录询问，重建会清空询问，每次求解前清空统计量。
建表 O(n log n)，q 次询问排序 O(q log q)，移动 O(n sqrt(q) + q) 次。
文件尾第一段替换结构体中的 3 个成员声明，示例统计路径颜色数。颜色先离散到 1..m，按查询编号读 ans。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

struct MoTree {
    struct Query {
        int l, r, extra, id;
    };
    int n, lg, timer;
    V < V < int > > g, up;
    V < int > in, out, ord, dep;
    V < Query > qs;

    void add(int u);
    void del(int u);
    void answer(int id);

    MoTree(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        qs.clear();
    }
    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void dfs(int u, int fa) {
        up[0][u] = fa;
        dep[u] = dep[fa] + 1;
        for (int j = 1; j < lg; j ++) up[j][u] = up[j - 1][up[j - 1][u]];
        in[u] = ++ timer;
        ord[timer] = u;
        for (int v : g[u])
            if (v != fa) dfs(v, u);
        out[u] = ++ timer;
        ord[timer] = u;
    }
    void build(int root = 1) {
        lg = 1;
        while ((1ll << lg) <= n) lg ++;
        up.assign(lg, V < int >(n + 1));
        in.assign(n + 1, 0);
        out = dep = in;
        ord.assign(2 * n + 1, 0);
        timer = 0;
        qs.clear();
        if (n) dfs(root, 0);
    }
    int lca(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        int d = dep[u] - dep[v];
        for (int j = 0; j < lg; j ++)
            if (d >> j & 1) u = up[j][u];
        if (u == v) return u;
        for (int j = lg - 1; j >= 0; j --)
            if (up[j][u] != up[j][v]) u = up[j][u], v = up[j][v];
        return up[0][u];
    }
    int query(int u, int v) {
        if (in[u] > in[v]) swap(u, v);
        int z = lca(u, v), id = int(qs.size());
        if (z == u)
            qs.push_back({in[u], in[v], 0, id});
        else
            qs.push_back({out[u], in[v], z, id});
        return id;
    }
    void solve() {
        auto queries = qs;
        int B = max(1, int(2 * n / max(1.0, sqrt(double(qs.size())))));
        sort(queries.begin(), queries.end(), [&](Query a, Query b) {
            if (a.l / B != b.l / B) return a.l / B < b.l / B;
            return (a.l / B & 1) ? a.r > b.r : a.r < b.r;
        });
        V < int > vis(n + 1);
        auto toggle = [&](int u) {
            if (vis[u])
                del(u);
            else
                add(u);
            vis[u] ^= 1;
        };
        int l = 1, r = 0;
        for (auto q : queries) {
            while (l > q.l) toggle(ord[-- l]);
            while (r < q.r) toggle(ord[++ r]);
            while (l < q.l) toggle(ord[l ++]);
            while (r > q.r) toggle(ord[r --]);
            if (q.extra) toggle(q.extra);
            answer(q.id);
            if (q.extra) toggle(q.extra);
        }
    }
};

/*
V < int > color, cnt, ans; int kinds = 0;
void add(int u) { if (cnt[color[u]] ++ == 0) ++ kinds; }
void del(int u) { if (-- cnt[color[u]] == 0) -- kinds; }
void answer(int id) { ans[id] = kinds; }
*/

/*
MoTree mo; mo.init(n);
for (auto [u, v] : edges) mo.add_edge(u, v);
mo.build(); int p = mo.lca(u, v); int id = mo.query(u, v);
mo.color = color; mo.cnt.assign(m + 1, 0); mo.kinds = 0;
mo.ans.resize(mo.qs.size()); mo.solve();
*/
