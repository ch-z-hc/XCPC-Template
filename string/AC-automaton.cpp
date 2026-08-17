#include <bits/stdc++.h>
using namespace std;

// 小写字母非空模式串的多模式匹配；add_pattern 返回模式编号（0-based）。
// build 复杂度 O(状态数 * 字符集大小)，匹配复杂度 O(文本长度 + 状态数 + 模式数)。
struct AhoCorasick {
    struct Node {
        array<int, 26> child{}, transition{};
        int fail = 0;
        vector<int> output;
    };
    vector<Node> trie;
    vector<int> pattern_terminal;
    vector<int> bfs_order;

    AhoCorasick() { trie.emplace_back(); }
    void clear() { trie.assign(1, Node{}); pattern_terminal.clear(); bfs_order.clear(); }
    int add_pattern(const string &pattern) {
        int u = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            if (!trie[u].child[c]) trie[u].child[c] = int(trie.size()), trie.emplace_back();
            u = trie[u].child[c];
        }
        int id = int(pattern_terminal.size());
        pattern_terminal.push_back(u); trie[u].output.push_back(id);
        return id;
    }
    void build() {
        queue<int> q; bfs_order = {0};
        for (Node &node : trie) {
            node.fail = 0;
            node.transition = node.child;
        }
        for (int c = 0; c < 26; ++c)
            if (trie[0].child[c]) q.push(trie[0].child[c]);
        while (!q.empty()) {
            int u = q.front(); q.pop(); bfs_order.push_back(u);
            for (int c = 0; c < 26; ++c) {
                int v = trie[u].child[c];
                if (v) {
                    trie[v].fail = trie[trie[u].fail].transition[c];
                    trie[u].transition[c] = v;
                    q.push(v);
                } else {
                    trie[u].transition[c] = trie[trie[u].fail].transition[c];
                }
            }
        }
    }
    vector<long long> count_occurrences(const string &text) const {
        vector<long long> answer(pattern_terminal.size()), state_count(trie.size());
        int u = 0;
        for (char ch : text) { u = trie[u].transition[ch - 'a']; ++state_count[u]; }
        for (int i = int(bfs_order.size()) - 1; i > 0; --i) {
            int v = bfs_order[i];
            state_count[trie[v].fail] += state_count[v];
        }
        for (int id = 0; id < int(pattern_terminal.size()); ++id) answer[id] = state_count[pattern_terminal[id]];
        return answer;
    }
};
