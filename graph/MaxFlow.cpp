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
    int n, m, s, t, tot = 1; cin >> n >> m >> s >> t;
    V < int > head(n + 1), now(n + 1), dep(n + 1);
    V < int > nxt(2 * m + 1), to(2 * m + 1), val(2 * m + 1);
    auto add = [&](int u, int v, int w) {
        nxt[++ tot] = head[u];
        head[u] = tot;
        to[tot] = v;
        val[tot] = w;
    };

    for (int i = 1; i <= m; ++ i) {
        int u, v, w; cin >> u >> v >> w;
        add(u, v, w); add(v, u, 0);
    }

    auto bfs = [&]() -> bool {
        for (auto &x : dep) x = 0;
        dep[s] = 1; now[s] = head[s];
    
        std::queue < int > Q;
        Q.push(s);
    
        while (Q.size()) {
            int u = Q.front();
            Q.pop();
    
            for (int i = head[u]; i; i = nxt[i]) {
                int v = to[i];
                if (val[i] && ! dep[v]) {
                    dep[v] = dep[u] + 1;
                    Q.push(v); now[v] = head[v];
                }
            }
        }
        return dep[t];
    };

    auto dinic = [&](auto&&self, int u, int flow) -> int {
        if (u == t) return flow;
        int ret = 0; int i = now[u];
        for (; i && flow; i = nxt[i]) {
            int v = to[i];
            if (val[i] && dep[v] == dep[u] + 1) {
                int k = self(self, v, min(flow, val[i]));
                flow -= k;
                ret += k;
                val[i] -= k;
                val[i ^ 1] += k;
                if (flow == 0) break;
            }
        }
        now[u] = i;
        return ret;
    };

    ll maxflow = 0; int flow = 0;
    while (bfs()) while ((flow = dinic(dinic, s, INF))) maxflow += flow;
    cout << maxflow << '\n';
    return 0;
}