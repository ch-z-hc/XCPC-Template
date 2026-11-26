/*
求差分约束的一组可行解，O(nm)。松弛的中间量须能放入 i64，包括检测负环的最后一轮。
add(u, v, w) 表示 x[u] - x[v] <= w，solve 无解时返回 false。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct Diff {
    int n = 0;
    std::vector<std::tuple<int, int, i64>> edges;

    Diff(int n = 0) : n(n) {}
    void init(int n_) {
        n = n_;
        edges.clear();
    }
    void add(int u, int v, i64 w) { edges.push_back({u, v, w}); }

    bool solve(std::vector<i64> &dis) {
        dis.assign(n + 1, 0);
        for (int i = 1; i <= n; i ++) {
            bool op = false;
            for (auto [u, v, w] : edges) {
                if (dis[u] > dis[v] + w) {
                    dis[u] = dis[v] + w;
                    op = true;
                }
            }
            if (!op) break;
            if (i == n) return false;
        }
        return true;
    }
};

/*
Diff dc; dc.init(2); dc.add(1, 2, -3);
std::vector<i64> x; if (dc.solve(x)) cout << x[1];
*/
