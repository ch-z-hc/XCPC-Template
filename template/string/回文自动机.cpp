#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

/*
小写字母回文自动机，支持末尾追加、回文串去重和出现次数统计。建树、统计 O(n)，查询种数 O(1)。
节点 0、1 是虚根。
*/

struct PAM {
    struct Node {
        array < int, 26 > ch{};
        int len = 0, fail = 0;
        ll cnt = 0;
    };

    V < Node > tr;
    string s;
    int last = 0;

    PAM() { init(); }
    PAM(const string &s) { build(s); }

    void init() {
        tr.assign(2, {});
        tr[0].fail = 1; tr[1].len = -1;
        s = "#"; last = 0;
    }

    int find(int u) {
        int p = int(s.size()) - 1;
        while (s[p - tr[u].len - 1] != s[p]) u = tr[u].fail;
        return u;
    }

    int insert(char c) {
        s += c;
        int x = c - 'a', u = find(last);
        if (! tr[u].ch[x]) {
            int v = int(tr.size());
            tr.push_back({});
            tr[v].len = tr[u].len + 2;
            tr[v].fail = tr[find(tr[u].fail)].ch[x];
            tr[u].ch[x] = v;
        }
        last = tr[u].ch[x];
        tr[last].cnt ++;
        return last;
    }

    void build(const string &str) {
        init();
        for (char c : str) insert(c);
    }

    int distinct() { return int(tr.size()) - 2; }

    V < ll > count() {
        V < ll > ans(tr.size());
        for (int i = 2; i < int(tr.size()); ++ i) ans[i] = tr[i].cnt;
        for (int i = int(tr.size()) - 1; i >= 2; -- i)
            ans[tr[i].fail] += ans[i];
        ans[0] = ans[1] = 0;
        return ans;
    }
};

/*
PAM pam; pam.build("ab"); int u = pam.insert('a'); // 最长回文后缀 aba 的节点
auto cnt = pam.count(); int kinds = pam.distinct(); // cnt[u] = 1，共 3 种非空回文串
pam.init();
*/
