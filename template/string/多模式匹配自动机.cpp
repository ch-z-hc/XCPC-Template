/*
小写字母多模式匹配，模式非空，先插入再建表，追加模式后须重建。
S 个节点、p 个模式串，建表 O(26S)，单次文本查询 O(|s| + S + p)。
query 按模式插入编号返回出现次数。
*/

#include <bits/stdc++.h>

using i64 = long long;

struct AC {
    struct Node {
        std::array<int, 26> ch{}, go{};
        int fail = 0;
    };

    std::vector<Node> tr;
    std::vector<int> pos, ord;

    AC() { clear(); }

    void clear() {
        tr.assign(1, Node{});
        pos.clear();
        ord.clear();
    }

    int insert(const std::string &s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!tr[u].ch[x]) {
                tr[u].ch[x] = int(tr.size());
                tr.emplace_back();
            }
            u = tr[u].ch[x];
        }
        int id = int(pos.size());
        pos.push_back(u);
        return id;
    }

    void build() {
        ord = {0};
        for (int i = 0; i < int(ord.size()); i ++) {
            int u = ord[i];
            for (int c = 0; c < 26; c ++) {
                int v = tr[u].ch[c];
                int fallback = u ? tr[tr[u].fail].go[c] : 0;
                tr[u].go[c] = v ? v : fallback;
                if (v) {
                    tr[v].fail = fallback;
                    ord.push_back(v);
                }
            }
        }
    }

    std::vector<i64> query(const std::string &s) {
        std::vector<i64> cnt(tr.size()), ans(pos.size());
        int u = 0;
        for (char c : s) {
            u = tr[u].go[c - 'a'];
            cnt[u] ++;
        }
        for (int i = int(ord.size()) - 1; i > 0; i --) {
            int v = ord[i];
            cnt[tr[v].fail] += cnt[v];
        }
        for (int i = 0; i < int(pos.size()); i ++) ans[i] = cnt[pos[i]];
        return ans;
    }
};

/*
AC ac; int id = ac.insert("aba"); ac.build();
auto cnt = ac.query("ababa");
ac.clear();
*/
