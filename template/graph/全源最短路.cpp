#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 用 dis[1..n][1..n] 存边权，重边取最小，无边填 inf，对角填 0，有负自环时取其权值。
// floyd 原地更新距离。有负环时返回 false，受负环影响的点对没有有限最短路。
// 0 < inf <= 2^62，有限距离的绝对值须小于 inf。时间 O(n³)，额外空间 O(1)。
bool floyd(V < V < ll > > &dis, ll inf = (1ll << 62)) {
    int n = int(dis.size()) - 1;
    for (int k = 1; k <= n; ++ k) {
        for (int i = 1; i <= n; ++ i) {
            if (dis[i][k] >= inf) continue;
            for (int j = 1; j <= n; ++ j) {
                if (dis[k][j] >= inf) continue;
                dis[i][j] = max(-inf, min(dis[i][j], dis[i][k] + dis[k][j]));
            }
        }
    }
    for (int i = 1; i <= n; ++ i) if (dis[i][i] < 0) return false;
    return true;
}

// bool ok = floyd(dis);
