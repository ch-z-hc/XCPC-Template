#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 全源最短路：矩阵 1-based；无边填 inf，对角填 min(0, 自环边权)。
// 重边先取最小；允许负边，无负环时更新 dis 为最短距离。
// 返回 false 表示存在负环，此时受负环影响的距离不可用。
// O(n^3) 时间、O(1) 额外空间；0 < inf <= 2^62，有限距离绝对值 < inf。
bool floyd(V < V < ll > > &dis, ll inf = (1ll << 62)) {
    int n = int(dis.size()) - 1;
    for (int k = 1; k <= n; ++ k)
        for (int i = 1; i <= n; ++ i) if (dis[i][k] < inf)
            for (int j = 1; j <= n; ++ j) if (dis[k][j] < inf)
                dis[i][j] = max(-inf, min(dis[i][j], dis[i][k] + dis[k][j]));
    for (int i = 1; i <= n; ++ i) if (dis[i][i] < 0) return false;
    return true;
}

// bool ok = floyd(dis); // dis 在原矩阵上更新。
