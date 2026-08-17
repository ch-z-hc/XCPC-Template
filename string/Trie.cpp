#include <bits/stdc++.h>
using namespace std;

// 小写字母 Trie；字符串下标和内部节点编号 0-based。
// 单次插入和查询复杂度 O(字符串长度)，空间与所有插入字符串总长度同阶。
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
