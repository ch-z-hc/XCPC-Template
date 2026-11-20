#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// add(u, v, w) 加入 x[u] - x[v] <= w，变量编号 1..n。
// solve(dis) 有解时把一组可行赋值写入 dis，无解返回 false。
// 松弛时的 dis[v] + w 要能放进 ll，包括判负环的那一轮。时间 O(nm)，空间 O(n + m)。

struct Diff {
    int n = 0;
    V < tuple < int, int, ll > > edges;

    Diff(int n = 0) : n(n) {}
    void init(int n_) { n = n_; edges.clear(); }
    void add(int u, int v, ll w) { edges.push_back({u, v, w}); }

    // 返回 false 表示存在负环（无解），否则 dis 是一组可行解。
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

// Diff dc(n);
// dc.add(u, v, w);
// V < ll > dis;
// bool ok = dc.solve(dis);
