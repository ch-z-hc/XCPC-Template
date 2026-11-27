/*
维护多串的不同子串并集，小写字母。根为 0，insert 可在线插入整串，extend 从已有前缀续接。
build 按字典树层序离线扩展；总长 n、字母表大小固定时建树 O(n)，匹配 O(|s|)，种数 O(状态数)。
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

struct GSAM {
    struct Node {
        array < int, 26 > ch{};
        int len = 0, fa = -1;
    };
    V < Node > tr = V < Node >(1);

    int split(int p, int c) {
        int q = tr[p].ch[c];
        if (tr[q].len == tr[p].len + 1) return q;
        int v = int(tr.size());
        tr.push_back(tr[q]);
        tr[v].len = tr[p].len + 1;
        tr[q].fa = v;
        while (p >= 0 && tr[p].ch[c] == q) {
            tr[p].ch[c] = v;
            p = tr[p].fa;
        }
        return v;
    }

    int extend(int p, char ch) {
        int c = ch - 'a';
        if (tr[p].ch[c]) return split(p, c);
        int v = int(tr.size());
        tr.emplace_back();
        tr[v].len = tr[p].len + 1;
        while (p >= 0 && !tr[p].ch[c]) {
            tr[p].ch[c] = v;
            p = tr[p].fa;
        }
        tr[v].fa = p < 0 ? 0 : split(p, c);
        return v;
    }

    void insert(const string &s) {
        int p = 0;
        for (char c : s) p = extend(p, c);
    }
    void build(const V < string > &words) {
        tr.assign(1, {});
        V < array < int, 26 > > ch(1);
        for (auto &s : words) {
            int u = 0;
            for (char c : s) {
                int x = c - 'a';
                if (!ch[u][x]) {
                    ch[u][x] = int(ch.size());
                    ch.emplace_back();
                }
                u = ch[u][x];
            }
        }
        V < int > ord{0}, id(ch.size());
        for (size_t i = 0; i < ord.size(); i ++) {
            int u = ord[i];
            for (int c = 0; c < 26; c ++) if (int v = ch[u][c]) {
                id[v] = extend(id[u], char('a' + c));
                ord.push_back(v);
            }
        }
    }
    bool contains(const string &s) {
        int u = 0;
        for (char c : s) if (!(u = tr[u].ch[c - 'a'])) return false;
        return true;
    }
    ll distinct() {
        ll ans = 0;
        for (int i = 1; i < int(tr.size()); i ++)
            ans += tr[i].len - tr[tr[i].fa].len;
        return ans;
    }
};

/*
GSAM sam; sam.build({"aba", "bab"}); sam.insert("abc");
int p = sam.extend(0, 'b'); bool ok = sam.contains("ba");
ll n = sam.distinct();
*/
