/*
小写字母后缀自动机，支持末尾追加、子串出现次数统计。建树、统计、求最大「长度 × 次数」均为 O(n)。
count 按状态返回出现次数。query 只考虑至少出现两次的子串。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct SAM {
    struct Node {
        std::array<int, 26> ch{};
        int fa = -1, len = 0;
        i64 cnt = 0;
    };

    std::vector<Node> tr;
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
        while (p >= 0 && !tr[p].ch[x]) {
            tr[p].ch[x] = cur;
            p = tr[p].fa;
        }
        if (p < 0)
            tr[cur].fa = 0;
        else {
            int q = tr[p].ch[x];
            if (tr[p].len + 1 == tr[q].len)
                tr[cur].fa = q;
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

    void build(const std::string &s) {
        clear();
        for (char c : s) extend(c);
    }

    std::vector<i64> count() {
        int n = 0;
        for (auto &p : tr) n = std::max(n, p.len);
        std::vector<int> cnt(n + 1), ord(tr.size());
        for (auto &p : tr) cnt[p.len] ++;
        for (int i = 1; i <= n; i ++) cnt[i] += cnt[i - 1];
        for (int i = int(tr.size()) - 1; i >= 0; i --) ord[-- cnt[tr[i].len]] = i;
        std::vector<i64> ans(tr.size());
        for (int i = 0; i < int(tr.size()); i ++) ans[i] = tr[i].cnt;
        for (int i = int(ord.size()) - 1; i > 0; i --) ans[tr[ord[i]].fa] += ans[ord[i]];
        return ans;
    }

    i64 query() {
        auto cnt = count();
        i64 ans = 0;
        for (int i = 0; i < int(tr.size()); i ++)
            if (cnt[i] > 1) ans = std::max(ans, cnt[i] * tr[i].len);
        return ans;
    }
};

/*
SAM sam; sam.build("aba"); sam.extend('b');
auto cnt = sam.count();
i64 best = sam.query();
sam.clear();
*/
