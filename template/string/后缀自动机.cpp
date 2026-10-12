#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 小写字母后缀自动机；字符串和状态下标 0-based。
// build、count 和 query 均为 O(n)，空间 O(n * 26)。
// extend 记录原始出现次数，查询使用副本累加，允许重复查询。
// count 返回各状态的出现次数；query 返回重复子串的最大（长度 * 出现次数）。
struct SAM {
    struct Node {
        array < int, 26 > ch{};
        int fa = -1, len = 0;
        ll cnt = 0;
    };

    V < Node > tr;
    int last = 0;

    SAM() { clear(); }

    void clear() {
        tr.assign(1, Node{});
        last = 0;
    }

    void extend(char c) {
        int x = c - 'a', cur = int(tr.size());
        tr.emplace_back();
        tr[cur].len = tr[last].len + 1;
        tr[cur].cnt = 1;
        int p = last;
        while (p >= 0 && ! tr[p].ch[x]) {
            tr[p].ch[x] = cur;
            p = tr[p].fa;
        }
        if (p < 0) tr[cur].fa = 0;
        else {
            int q = tr[p].ch[x];
            if (tr[p].len + 1 == tr[q].len) tr[cur].fa = q;
            else {
                int clone = int(tr.size());
                tr.push_back(tr[q]);
                tr[clone].len = tr[p].len + 1;
                tr[clone].cnt = 0;
                while (p >= 0 && tr[p].ch[x] == q) {
                    tr[p].ch[x] = clone;
                    p = tr[p].fa;
                }
                tr[q].fa = tr[cur].fa = clone;
            }
        }
        last = cur;
    }

    void build(const string &s) {
        clear();
        for (char c : s) extend(c);
    }

    V < ll > count() {
        int n = 0;
        for (auto &p : tr) n = max(n, p.len);
        V < int > cnt(n + 1), ord(tr.size());
        for (auto &p : tr) cnt[p.len] ++;
        for (int i = 1; i <= n; ++ i) cnt[i] += cnt[i - 1];
        for (int i = int(tr.size()) - 1; i >= 0; -- i)
            ord[-- cnt[tr[i].len]] = i;
        V < ll > ans(tr.size());
        for (int i = 0; i < int(tr.size()); ++ i) ans[i] = tr[i].cnt;
        for (int i = int(ord.size()) - 1; i > 0; -- i)
            ans[tr[ord[i]].fa] += ans[ord[i]];
        return ans;
    }

    ll query() {
        auto cnt = count();
        ll ans = 0;
        for (int i = 0; i < int(tr.size()); ++ i)
            if (cnt[i] > 1) ans = max(ans, cnt[i] * tr[i].len);
        return ans;
    }
};

// 调用示例：
// SAM sam;
// sam.build(s);
// ll ans = sam.query();
