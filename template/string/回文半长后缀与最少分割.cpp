/*
先复制「回文自动机」。half[u] 是长度至多为节点 u 一半的最长回文后缀，空后缀为 0。
cut[i] 是前 i 个字符的最少回文段数，series 按相邻回文后缀长度差分组。
支持在线追加；倍增求半长后缀，n 次追加共 O(n log n) 时间与空间。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

struct PalPartition {
    PAM pam;
    V < int > half, series, diff, cut;

    V < V < int > > up;
    V < int > best;

    PalPartition() { init(); }
    void init() {
        pam.init();
        half.assign(2, 0); series.assign(2, 0); diff.assign(2, 0);
        best.assign(2, 0); up.assign(2, {0});
        cut = {0};
    }

    int insert(char c) {
        int u = pam.insert(c);
        auto &tr = pam.tr;
        if (u == int(half.size())) {
            int v = tr[u].fail, len = tr[u].len;
            up.push_back({v});
            for (int j = 1; (1ll << j) <= len; j ++) {
                int a = up[u][j - 1];
                up[u].push_back(j <= int(up[a].size()) ? up[a][j - 1] : 0);
            }
            int a = u;
            for (int j = int(up[u].size()) - 1; j >= 0; j --) {
                if (j < int(up[a].size()) && tr[up[a][j]].len > len / 2)
                    a = up[a][j];
            }
            half.push_back(tr[a].fail);
            diff.push_back(len - tr[v].len);
            series.push_back(diff[u] == diff[v] ? series[v] : v);
            best.push_back(0);
        }
        int i = int(cut.size());
        cut.push_back(i);
        for (int p = u; p >= 2; p = series[p]) {
            int v = tr[p].fail;
            best[p] = cut[i - tr[series[p]].len - diff[p]];
            if (diff[p] == diff[v]) best[p] = min(best[p], best[v]);
            cut[i] = min(cut[i], best[p] + 1);
        }
        return u;
    }

    void build(const string &s) {
        init();
        for (char c : s) insert(c);
    }
};

/*
PalPartition p; p.build("abacd"); int u = p.insert('c');
int pieces = p.cut.back(), half = p.half[u];
p.init();
*/
