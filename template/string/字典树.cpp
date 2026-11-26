/*
小写字母串的前缀和整串计数，均含重复项，允许空串。插入和查询 O(串长)。
find 返回前缀节点，不存在时返回 -1。
*/

#include <bits/stdc++.h>

struct Trie {
    struct Node {
        std::array<int, 26> ch{};
        int cnt = 0, end = 0;
    };

    std::vector<Node> tr;

    Trie() { clear(); }
    void clear() { tr.assign(1, Node{}); }

    void insert(const std::string &s) {
        int u = 0;
        tr[u].cnt ++;
        for (char c : s) {
            int x = c - 'a';
            if (!tr[u].ch[x]) {
                tr[u].ch[x] = int(tr.size());
                tr.emplace_back();
            }
            u = tr[u].ch[x];
            tr[u].cnt ++;
        }
        tr[u].end ++;
    }

    int find(const std::string &s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!tr[u].ch[x]) return -1;
            u = tr[u].ch[x];
        }
        return u;
    }

    int prefix(const std::string &s) {
        int u = find(s);
        return u == -1 ? 0 : tr[u].cnt;
    }

    int count(const std::string &s) {
        int u = find(s);
        return u == -1 ? 0 : tr[u].end;
    }
};

/*
Trie trie; trie.insert("aba"); int node = trie.find("ab");
int pre = trie.prefix("ab"), exact = trie.count("ab");
trie.clear();
*/
