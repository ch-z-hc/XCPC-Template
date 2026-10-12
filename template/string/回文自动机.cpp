#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 回文树，只接收小写字母；字符串对外 0-based，内部添加一个哨兵。
// 建树 O(n)，空间 O(26n)；count() 返回各节点代表的回文串出现次数。
// 0、1 为长度 0、-1 的虚根，其余节点对应不同的非空回文串。

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

// 调用示例：PAM pam(s); auto cnt = pam.count(); int tot = pam.distinct();
