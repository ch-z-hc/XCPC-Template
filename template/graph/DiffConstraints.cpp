#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 差分约束：add(u, v, w) 表示 x[u] - x[v] <= w。
// 变量编号 1-based；Bellman-Ford 复杂度 O(nm)，空间 O(n + m)。
// 权值支持整个 ll 范围；内部使用 __int128_t，需 GCC / Clang。
// 可行解必要时整体平移；不存在 ll 范围内的可行解时抛 overflow_error。

struct Diff {
    int n = 0;
    V < tuple < int, int, ll > > edges;

    Diff(int n = 0) : n(n) {}
    void init(int n_) { n = n_; edges.clear(); }
    void add(int u, int v, ll w) { edges.push_back({u, v, w}); }

    // 返回 false 表示存在负环（无解），否则 dis 是一组可行解。
    bool solve(V < ll > &dis) const {
        dis.assign(n + 1, 0);
        V < __int128_t > val(n + 1, 0);
        for (int i = 1; i <= n; ++ i) {
            bool op = false;
            for (auto [u, v, w] : edges) {
                if (val[u] > val[v] + w) {
                    val[u] = val[v] + w;
                    op = true;
                }
            }
            if (! op) break;
            if (i == n) return false;
        }
        __int128_t shift = 0;
        for (int u = 1; u <= n; ++ u)
            shift = max(shift, __int128_t(LLONG_MIN) - val[u]);
        if (shift > LLONG_MAX)
            throw overflow_error("Difference constraint solution exceeds ll");
        for (int u = 1; u <= n; ++ u) dis[u] = static_cast<ll> (val[u] + shift);
        return true;
    }
};

// 调用示例：
// Diff dc(n);
// dc.add(u, v, w);
// V < ll > dis;
// bool op = dc.solve(dis);
