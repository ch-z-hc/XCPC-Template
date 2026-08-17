#include <bits/stdc++.h>
using namespace std;

// 小写字母后缀自动机；字符串和状态下标 0-based。
// build 与 occurrence_count 均为 O(n)，状态数和空间为 O(n)。
struct SuffixAutomaton {
    struct State { array<int, 26> next{}; int link = -1, len = 0; long long occurrence = 0; };
    vector<State> state;
    int last = 0;

    SuffixAutomaton() { state.emplace_back(); }
    void clear() { state.assign(1, State{}); last = 0; }
    void extend(char ch) {
        int c = ch - 'a', cur = int(state.size()); state.emplace_back();
        state[cur].len = state[last].len + 1; state[cur].occurrence = 1;
        int p = last;
        while (p >= 0 && !state[p].next[c]) state[p].next[c] = cur, p = state[p].link;
        if (p < 0) state[cur].link = 0;
        else {
            int q = state[p].next[c];
            if (state[p].len + 1 == state[q].len) state[cur].link = q;
            else {
                int clone = int(state.size()); state.push_back(state[q]);
                state[clone].len = state[p].len + 1; state[clone].occurrence = 0;
                while (p >= 0 && state[p].next[c] == q) state[p].next[c] = clone, p = state[p].link;
                state[q].link = state[cur].link = clone;
            }
        }
        last = cur;
    }
    void build(const string &s) { clear(); for (char ch : s) extend(ch); }
    vector<long long> occurrence_count() {
        int max_len = 0; for (auto &x : state) max_len = max(max_len, x.len);
        vector<int> count(max_len + 1), order(state.size());
        for (auto &x : state) ++count[x.len];
        for (int i = 1; i <= max_len; ++i) count[i] += count[i - 1];
        for (int i = int(state.size()) - 1; i >= 0; --i) order[--count[state[i].len]] = i;
        vector<long long> answer(state.size());
        for (int i = 0; i < int(state.size()); ++i) answer[i] = state[i].occurrence;
        for (int i = int(order.size()) - 1; i > 0; --i)
            answer[state[order[i]].link] += answer[order[i]];
        return answer;
    }
    long long longest_repeated_product() {
        auto occurrence = occurrence_count(); long long answer = 0;
        for (int i = 0; i < int(state.size()); ++i) if (occurrence[i] > 1)
            answer = max(answer, occurrence[i] * state[i].len);
        return answer;
    }
};
