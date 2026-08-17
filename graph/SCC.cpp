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

int main() {
    ios::sync_with_stdio(0); 
    cin.tie(0);
    int n, m, dt = 0, ct = 0, st = 0;
    cin >> n >> m;
    V < int > a(n + 1), b(n + 1);
    V < int > dfn(n + 1), low(n + 1);
    V < int > col(n + 1), sck(n + 1);
    V < V < int > > nxt(n + 1);

    for (int i = 1; i <= n; ++ i) cin >> a[i];
    for (int i = 1; i <= m; ++ i) {
        int u, v; cin >> u >> v;
        nxt[u].push_back(v);
    }

    auto tarjan = [&](auto&&self, int u) -> void {
        ++ dt; dfn[u] = low[u] = dt;
        ++ st; sck[st] = u;
        for (auto v : nxt[u]) {
            if (! dfn[v]) {
                self(self, v);  
                low[u] = min(low[u], low[v]); 
            }
            else if (! col[v])  
                low[u] = min(low[u], dfn[v]);
        }
    
        if (dfn[u] == low[u]) {
            ++ ct;
            while (sck[st] != u) {
                b[ct] += a[sck[st]]; col[sck[st]] = ct; st --;
            }
            b[ct] += a[sck[st]]; col[sck[st]] = ct; st --;
        }
    };

    for (int i = 1; i <= n; ++ i)
        if (! dfn[i]) tarjan(tarjan, i);

    V < int > ans(ct + 1), in(ct + 1);
    V < V < int > > nxt2(ct + 1);

    for (int u = 1; u <= n; ++ u) {
        for (auto v : nxt[u]) {
            if (col[u] != col[v]) {
                nxt2[col[u]].push_back(col[v]);
                in[col[v]] ++; 
            }
        }
    }

    queue < int > Q;
    for (int i = 1; i <= ct; ++ i) {
        if (in[i] == 0) {
            Q.push(i); ans[i] = b[i];
        }
    }

    while (Q.size()) {
        int u = Q.front(); Q.pop();
        for (auto v : nxt2[u]) {
            ans[v] = max(ans[v], ans[u] + b[v]);
            in[v] --;
            if (in[v] == 0) Q.push(v);
        }
    } 

    int fans = 0;
    for (int i = 1; i <= ct; ++ i)
        fans = std::max(fans, ans[i]);

    cout << fans << '\n';
    return 0;
}