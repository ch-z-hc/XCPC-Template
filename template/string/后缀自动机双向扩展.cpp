/*
先复制「后缀自动机」。在固定小写文本上定位子串，状态用 (节点, 已匹配长度)。
从 (0, 0) 开始，可任意向左或右加一个字符；失败返回 (-1, 0)。建表 O(n)，单次扩展 O(1)。
最长公共子串按文本顺序扫描，时间 O(|t|)。
*/

#include <bits/stdc++.h>

#define V vector
#define fi first
#define se second
#define pii pair < int, int >
using namespace std;

struct BiSAM {
    SAM sam;
    string s;
    V < int > pos;
    V < array < int, 26 > > left;

    void build(const string &text) {
        s = text;
        sam.clear();
        pos = {-1};
        for (int i = 0; i < int(s.size()); i ++) {
            sam.extend(s[i]);
            pos.resize(sam.tr.size(), -1);
            pos[sam.last] = i;
        }
        V < V < int > > child(pos.size());
        for (int v = 1; v < int(pos.size()); v ++) child[sam.tr[v].fa].push_back(v);
        left.assign(pos.size(), {});
        function < void(int) > dfs = [&](int u) {
            for (int v : child[u]) {
                dfs(v);
                pos[u] = pos[v];
                left[u][s[pos[v] - sam.tr[u].len] - 'a'] = v;
            }
        };
        dfs(0);
    }

    pii prepend(pii state, char c) {
        auto [u, len] = state;
        if (u < 0) return {-1, 0};
        if (len == sam.tr[u].len) {
            u = left[u][c - 'a'];
            if (!u) return {-1, 0};
        } else if (s[pos[u] - len] != c) return {-1, 0};
        return {u, len + 1};
    }
    pii append(pii state, char c) {
        int u = state.fi;
        if (u < 0 || !(u = sam.tr[u].ch[c - 'a'])) return {-1, 0};
        return {u, state.se + 1};
    }
    int lcs(const string &t) {
        int u = 0, len = 0, best = 0;
        for (char c : t) {
            int x = c - 'a';
            while (u && !sam.tr[u].ch[x]) {
                u = sam.tr[u].fa; len = sam.tr[u].len;
            }
            if ((u = sam.tr[u].ch[x])) len ++;
            else len = 0;
            best = max(best, len);
        }
        return best;
    }
};

/*
BiSAM b; b.build("ababa");
auto state = b.append({0, 0}, 'b'); state = b.prepend(state, 'a');
int len = b.lcs("babb");
*/
