#include <bits/stdc++.h>
#include <cassert>

#define V vector

typedef long long ll;

using namespace std;

// 一般无向图最大权匹配，带权 Blossom；时间 O(n^3)，空间 O(n^2)。
// 顶点 1-based；允许未匹配，不优先最大基数；自环、非正权忽略。
// 重边保留最大权；solve 返回最大总权，match[u] 为匹配点，未匹配为 0。
// 每次 solve 从空匹配重算，init 清空图；无模数要求。
// 正边权 w <= LLONG_MAX / (8 * max(1, n))，保证对偶值和总权不溢出。
// 参考 bqi343/cp-notebook 的 GeneralWeightedMatch.h，CC0-1.0。
// github.com/bqi343/cp-notebook；保留收缩、展开和对偶调整的原算法。
struct WeightedBlossom {
    struct Edge { int u = 0, v = 0; ll w = 0; };
    int n = 0, nx = 0;
    ll timer = 0;
    V < V < Edge > > g;
    V < V < int > > flower, from;
    V < int > match, slack, base, fa, state;
    V < ll > lab, seen;
    queue < int > q;

    WeightedBlossom(int n = 0) { init(n); }

    void init(int n_) {
        n = n_; nx = n; timer = 0;
        int m = 2 * n + 1;
        g.assign(m, V < Edge > (m));
        from.assign(m, V < int > (n + 1));
        flower.assign(m, {});
        match.assign(m, 0); slack.assign(m, 0); base.assign(m, 0);
        fa.assign(m, 0); state.assign(m, -1);
        lab.assign(m, 0); seen.assign(m, 0);
        for (int u = 1; u <= n; ++ u)
            for (int v = 1; v <= n; ++ v) g[u][v] = {u, v, 0};
        q = queue < int > ();
    }

    void add_edge(int u, int v, ll w) {
        if (u == v || w <= 0) return;
        assert(w <= LLONG_MAX / (8ll * max(1, n)));
        g[u][v].w = g[v][u].w = max(g[u][v].w, w);
    }

    ll delta(Edge e) { return lab[e.u] + lab[e.v] - 2 * e.w; }

    void update_slack(int u, int x) {
        if (! slack[x] || delta(g[u][x]) < delta(g[slack[x]][x]))
            slack[x] = u;
    }

    void set_slack(int x) {
        slack[x] = 0;
        for (int u = 1; u <= n; ++ u)
            if (g[u][x].w > 0 && base[u] != x && state[base[u]] == 0)
                update_slack(u, x);
    }

    void push(int x) {
        if (x <= n) q.push(x);
        else for (int u : flower[x]) push(u);
    }

    void set_base(int x, int b) {
        base[x] = b;
        if (x > n) for (int u : flower[x]) set_base(u, b);
    }

    int position(int b, int x) {
        int p = int(find(flower[b].begin(), flower[b].end(), x)
                    - flower[b].begin());
        if (p & 1) {
            reverse(flower[b].begin() + 1, flower[b].end());
            p = int(flower[b].size()) - p;
        }
        return p;
    }

    void set_match(int u, int v) {
        Edge e = g[u][v]; match[u] = e.v;
        if (u <= n) return;
        int x = from[u][e.u], p = position(u, x);
        for (int i = 0; i < p; ++ i)
            set_match(flower[u][i], flower[u][i ^ 1]);
        set_match(x, v);
        rotate(flower[u].begin(), flower[u].begin() + p, flower[u].end());
    }

    void augment(int u, int v) {
        while (1) {
            int x = base[match[u]]; set_match(u, v);
            if (! x) return;
            set_match(x, base[fa[x]]);
            u = base[fa[x]]; v = x;
        }
    }

    int lca(int u, int v) {
        ++ timer;
        for (; u || v; swap(u, v)) {
            if (! u) continue;
            if (seen[u] == timer) return u;
            seen[u] = timer; u = base[match[u]];
            if (u) u = base[fa[u]];
        }
        return 0;
    }

    void contract(int u, int anc, int v) {
        int b = n + 1;
        while (b <= nx && base[b]) ++ b;
        if (b > nx) ++ nx;
        lab[b] = state[b] = 0; match[b] = match[anc];
        flower[b] = {anc};
        auto collect = [&](int x) {
            for (int y; x != anc; x = base[fa[y]]) {
                flower[b].push_back(x);
                flower[b].push_back(y = base[match[x]]); push(y);
            }
        };
        collect(u);
        reverse(flower[b].begin() + 1, flower[b].end());
        collect(v); set_base(b, b);
        for (int x = 1; x <= nx; ++ x) g[b][x].w = g[x][b].w = 0;
        fill(from[b].begin(), from[b].end(), 0);
        for (int s : flower[b]) {
            for (int x = 1; x <= nx; ++ x)
                if (! g[b][x].w || delta(g[s][x]) < delta(g[b][x])) {
                    g[b][x] = g[s][x]; g[x][b] = g[x][s];
                }
            for (int x = 1; x <= n; ++ x)
                if (from[s][x]) from[b][x] = s;
        }
        set_slack(b);
    }

    void expand(int b) {
        for (int x : flower[b]) set_base(x, x);
        int x = from[b][g[b][fa[b]].u], p = position(b, x);
        for (int i = 0; i < p; i += 2) {
            int u = flower[b][i], v = flower[b][i + 1];
            fa[u] = g[v][u].u; state[u] = 1;
            state[v] = slack[u] = slack[v] = 0; push(v);
        }
        state[x] = 1; fa[x] = fa[b];
        for (int i = p + 1; i < int(flower[b].size()); ++ i) {
            int u = flower[b][i]; state[u] = -1; set_slack(u);
        }
        base[b] = 0;
    }

    bool found(Edge e) {
        int u = base[e.u], v = base[e.v];
        if (state[v] == -1) {
            fa[v] = e.u; state[v] = 1; slack[v] = 0;
            int x = base[match[v]];
            state[x] = slack[x] = 0; push(x);
        } else if (state[v] == 0) {
            int anc = lca(u, v);
            if (! anc) { augment(u, v); augment(v, u); return 1; }
            contract(u, anc, v);
        }
        return 0;
    }

    bool matching() {
        q = queue < int > ();
        for (int x = 1; x <= nx; ++ x) {
            state[x] = -1; slack[x] = 0;
            if (base[x] == x && ! match[x]) {
                fa[x] = state[x] = 0; push(x);
            }
        }
        if (q.empty()) return 0;
        while (1) {
            while (! q.empty()) {
                int u = q.front(); q.pop();
                if (state[base[u]] == 1) continue;
                for (int v = 1; v <= n; ++ v)
                    if (g[u][v].w > 0 && base[u] != base[v]) {
                        if (! delta(g[u][v])) {
                            if (found(g[u][v])) return 1;
                        } else update_slack(u, base[v]);
                    }
            }
            ll d = LLONG_MAX / 4;
            for (int b = n + 1; b <= nx; ++ b)
                if (base[b] == b && state[b] == 1) d = min(d, lab[b] / 2);
            for (int x = 1; x <= nx; ++ x) if (base[x] == x && slack[x]) {
                ll w = delta(g[slack[x]][x]);
                if (state[x] == -1) d = min(d, w);
                else if (state[x] == 0) d = min(d, w / 2);
            }
            for (int u = 1; u <= n; ++ u) {
                if (state[base[u]] == 0) {
                    if (lab[u] <= d) return 0;
                    lab[u] -= d;
                } else if (state[base[u]] == 1) lab[u] += d;
            }
            for (int b = n + 1; b <= nx; ++ b)
                if (base[b] == b && state[b] != -1)
                    lab[b] += (state[b] == 0 ? 2 : -2) * d;
            q = queue < int > ();
            for (int x = 1; x <= nx; ++ x)
                if (base[x] == x && slack[x] && base[slack[x]] != x
                    && ! delta(g[slack[x]][x]))
                    if (found(g[slack[x]][x])) return 1;
            for (int b = n + 1; b <= nx; ++ b)
                if (base[b] == b && state[b] == 1 && ! lab[b]) expand(b);
        }
    }

    ll solve() {
        nx = n; timer = 0;
        fill(match.begin(), match.end(), 0);
        fill(base.begin(), base.end(), 0);
        fill(seen.begin(), seen.end(), 0);
        fill(lab.begin(), lab.end(), 0);
        for (auto &a : flower) a.clear();
        ll w = 0;
        for (int u = 1; u <= n; ++ u) {
            base[u] = u;
            for (int v = 1; v <= n; ++ v) {
                from[u][v] = u == v ? u : 0;
                w = max(w, g[u][v].w);
            }
        }
        for (int u = 1; u <= n; ++ u) lab[u] = w;
        while (matching()) {}
        ll ans = 0;
        for (int u = 1; u <= n; ++ u)
            if (match[u] > u) ans += g[u][match[u]].w;
        return ans;
    }
};

// 调用：WeightedBlossom blossom(n); blossom.add_edge(u, v, w);
// ll ans = blossom.solve(); 匹配边取所有 u < blossom.match[u]。
