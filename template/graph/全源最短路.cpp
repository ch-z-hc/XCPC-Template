#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
全源最短路，原地更新 1-based 距离矩阵，O(n³)。重边取最小，对角填 0 或更小的自环权。
无边填 inf，0 < inf <= 2^62，有限距离绝对值须小于 inf。有负环时返回 false。
*/

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
