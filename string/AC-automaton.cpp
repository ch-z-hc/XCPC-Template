#include <bits/stdc++.h>
using namespace std;

// 小写字母多模式匹配；add_pattern 返回模式编号（0-based）。
struct AhoCorasick {
    struct Node { array<int, 26> next{}; int fail = 0; vector<int> output; };
    vector<Node> trie;
    vector<int> pattern_terminal;
    vector<int> bfs_order;

    AhoCorasick() { trie.emplace_back(); }
    void clear() { trie.assign(1, Node{}); pattern_terminal.clear(); bfs_order.clear(); }
    int add_pattern(const string &pattern) {
        int u = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            if (!trie[u].next[c]) trie[u].next[c] = int(trie.size()), trie.emplace_back();
            u = trie[u].next[c];
        }
        int id = int(pattern_terminal.size());
        pattern_terminal.push_back(u); trie[u].output.push_back(id);
        return id;
    }
    void build() {
        queue<int> q; bfs_order = {0};
        for (int c = 0; c < 26; ++c) if (trie[0].next[c]) q.push(trie[0].next[c]);
        while (!q.empty()) {
            int u = q.front(); q.pop(); bfs_order.push_back(u);
            for (int c = 0; c < 26; ++c) {
                int &v = trie[u].next[c];
                if (v) trie[v].fail = trie[trie[u].fail].next[c], q.push(v);
                else v = trie[trie[u].fail].next[c];
            }
        }
    }
    vector<long long> count_occurrences(const string &text) const {
        vector<long long> answer(pattern_terminal.size()), state_count(trie.size());
        int u = 0;
        for (char ch : text) { u = trie[u].next[ch - 'a']; ++state_count[u]; }
        for (int i = int(bfs_order.size()) - 1; i > 0; --i) {
            int v = bfs_order[i];
            state_count[trie[v].fail] += state_count[v];
        }
        for (int id = 0; id < int(pattern_terminal.size()); ++id) answer[id] = state_count[pattern_terminal[id]];
        return answer;
    }
};
