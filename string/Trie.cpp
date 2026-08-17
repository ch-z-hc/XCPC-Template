#include <bits/stdc++.h>
using namespace std;

// 小写字母 Trie；prefix_count 查询以 prefix 开头的字符串数量。
struct Trie {
    struct Node { array<int, 26> next{}; int pass = 0, terminal = 0; };
    vector<Node> node;
    Trie() { node.emplace_back(); }
    void clear() { node.assign(1, Node{}); }
    void insert(const string &s) {
        int u = 0; ++node[u].pass;
        for (char ch : s) { int c = ch - 'a'; if (!node[u].next[c]) node[u].next[c] = int(node.size()), node.emplace_back(); u = node[u].next[c]; ++node[u].pass; }
        ++node[u].terminal;
    }
    int prefix_count(const string &s) const {
        int u = 0;
        for (char ch : s) { int c = ch - 'a'; if (!node[u].next[c]) return 0; u = node[u].next[c]; }
        return node[u].pass;
    }
    int word_count(const string &s) const {
        int u = 0;
        for (char ch : s) { int c = ch - 'a'; if (!node[u].next[c]) return 0; u = node[u].next[c]; }
        return node[u].terminal;
    }
};
