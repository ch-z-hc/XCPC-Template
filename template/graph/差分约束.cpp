#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 求差分约束的一组可行解，O(nm)。松弛的中间量须能放入 ll，包括检测负环的最后一轮。

struct Diff {
    int n = 0;
    V < tuple < int, int, ll > > edges;

    Diff(int n = 0) : n(n) {}
    void init(int n_) { n = n_; edges.clear(); }
    void add(int u, int v, ll w) { edges.push_back({u, v, w}); }

    bool solve(V < ll > &dis) {
        dis.assign(n + 1, 0);
        for (int i = 1; i <= n; ++ i) {
            bool op = false;
            for (auto [u, v, w] : edges) {
                if (dis[u] > dis[v] + w) {
                    dis[u] = dis[v] + w;
                    op = true;
                }
            }
            if (! op) break;
            if (i == n) return false;
        }
        return true;
    }
};

/*
Diff dc; dc.init(2); dc.add(1, 2, -3); // x[1] - x[2] <= -3
V < ll > x; if (dc.solve(x)) cout << x[1]; // 无解时返回 false
*/
