#include <bits/stdc++.h>

#define V vector

using namespace std;

// 小写字母字典树；字符串与节点编号 0-based，允许重复插入和空串。
// 单次插入、查询为 O(字符串长度)，空间 O(节点数 * 26)。
// prefix 返回此前缀的字符串数，count 返回完整字符串数；clear 清空。
struct Trie {
    struct Node {
        array < int, 26 > ch{};
        int cnt = 0, end = 0;
    };

    V < Node > tr;

    Trie() { clear(); }
    void clear() { tr.assign(1, Node{}); }

    void insert(const string &s) {
        int u = 0;
        tr[u].cnt ++;
        for (char c : s) {
            int x = c - 'a';
            if (! tr[u].ch[x]) {
                tr[u].ch[x] = int(tr.size());
                tr.emplace_back();
            }
            u = tr[u].ch[x];
            tr[u].cnt ++;
        }
        tr[u].end ++;
    }

    int find(const string &s) const {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (! tr[u].ch[x]) return -1;
            u = tr[u].ch[x];
        }
        return u;
    }

    int prefix(const string &s) const {
        int u = find(s);
        return u == -1 ? 0 : tr[u].cnt;
    }

    int count(const string &s) const {
        int u = find(s);
        return u == -1 ? 0 : tr[u].end;
    }
};

// 调用示例：
// Trie trie;
// trie.insert("aba");
// int cnt = trie.prefix("ab");
