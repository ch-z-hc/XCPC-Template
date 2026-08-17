#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << '\n'
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

const int mod = 1e9 + 7;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

const int INF = INT_MAX;

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n, m, s, t, ret = 0, tot = 1;
    cin >> n >> m >> s >> t;
    V < int > nxt(2 * m + 1), to(2 * m + 1), val1(2 * m + 1), val2(2 * m + 1);
    V < int > head(n + 1), now(n + 1), dis(n + 1), vis(n + 1);

    auto add = [&](int u, int v, int w1, int w2) {
        nxt[++ tot] = head[u];
        head[u] = tot;
        to[tot] = v;
        val1[tot] = w1;
        val2[tot] = w2;
    };

    for (int i = 1; i <= m; ++ i) {
        int u, v, w1, w2; cin >> u >> v >> w1 >> w2;
        // w2 = -w2;
        
        // w1 - flow
        // w2 - cost
        // 此模板为最小费用最大流
        // 如果要改为最大费用最大流则需要将 w2 改为负数
        // 需要保证原图无负圈

        add(u, v, w1, w2);
        add(v, u, 0, -w2);
    }

    auto spfa = [&]() -> bool {
        for (auto &x : dis) x = INF;
        dis[s] = 0;
        queue < int > Q;
        Q.push(s); now[s] = head[s];
    
        while (Q.size()) {
            int u = Q.front(); Q.pop();
            vis[u] = 0;
            for (int i = head[u]; i; i = nxt[i]) {
                int v = to[i];
                if (dis[v] > dis[u] + val2[i] && val1[i]) {
                    dis[v] = dis[u] + val2[i]; now[v] = head[v];
                    if (! vis[v]) Q.push(v), vis[v] = 1;
                }
            }
        }
    
        return dis[t] != INF;
    };

    auto dinic = [&](auto&&self, int u, int flow) -> int {
        if (u == t) return flow;
        if (vis[u]) return 0;
        vis[u] = 1;
        int rest = 0; int i = now[u];
    
        for (; i && flow; i = nxt[i]) {
            int v = to[i];
            if (dis[v] == dis[u] + val2[i] && ! vis[v] && val1[i]) {
                int k = self(self, v, min(flow, val1[i]));
                flow -= k;
                rest += k;
                ret += k * val2[i];
                val1[i] -= k;
                val1[i ^ 1] += k;
            }
        }
    
    
        now[u] = i; vis[u] = 0;
        return rest;
    };


    int flow = 0, maxflow = 0;
    while (spfa()) while ((flow = dinic(dinic, s, INF))) 
        maxflow += flow;
    
    cout << maxflow << ' ' << ret << '\n';
    return 0;
}