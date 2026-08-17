#include <bits/stdc++.h>
using namespace std;

// 后缀数组；字符串下标 0-based。build 复杂度 O(n log n)，lcp 查询 O(1)，空间 O(n log n)。
// suffix 按字典序保存后缀起点，rank[i] 是后缀 s[i..] 的排名。
struct SuffixArray {
    string text;
    vector<int> suffix, rank, height, log_table;
    vector<vector<int>> sparse_table;

    SuffixArray() = default;
    explicit SuffixArray(const string &s) { build(s); }

    void build(const string &s) {
        text = s;
        int n = int(text.size()), alphabet = 256;
        suffix.resize(n);
        rank.resize(n);
        height.assign(n, 0);
        log_table.assign(n + 1, 0);
        sparse_table.clear();
        if (n == 0) return;

        vector<int> old_rank(n), second_key(n), first_key(n), count(max(n, alphabet));
        for (int i = 0; i < n; ++i)
            ++count[rank[i] = static_cast<unsigned char>(text[i])];
        for (int i = 1; i < alphabet; ++i) count[i] += count[i - 1];
        for (int i = n - 1; i >= 0; --i) suffix[--count[rank[i]]] = i;

        for (int width = 1, classes;; width <<= 1, alphabet = classes) {
            classes = 0;
            for (int i = n - width; i < n; ++i)
                if (i >= 0) second_key[classes++] = i;
            for (int i = 0; i < n; ++i)
                if (suffix[i] >= width) second_key[classes++] = suffix[i] - width;

            fill(count.begin(), count.begin() + alphabet, 0);
            for (int i = 0; i < n; ++i) ++count[first_key[i] = rank[second_key[i]]];
            for (int i = 1; i < alphabet; ++i) count[i] += count[i - 1];
            for (int i = n - 1; i >= 0; --i)
                suffix[--count[first_key[i]]] = second_key[i];

            old_rank = rank;
            classes = 1;
            rank[suffix[0]] = 0;
            for (int i = 1; i < n; ++i) {
                int current = suffix[i], previous = suffix[i - 1];
                bool same = old_rank[current] == old_rank[previous];
                same = same && (current + width < n ? old_rank[current + width] : -1) ==
                                   (previous + width < n ? old_rank[previous + width] : -1);
                rank[current] = same ? classes - 1 : classes++;
            }
            if (classes == n) break;
        }

        for (int i = 2; i <= n; ++i) log_table[i] = log_table[i >> 1] + 1;
        for (int i = 0, common = 0; i < n; ++i) {
            if (rank[i] == 0) {
                common = 0;
                continue;
            }
            if (common) --common;
            int previous = suffix[rank[i] - 1];
            while (i + common < n && previous + common < n &&
                   text[i + common] == text[previous + common])
                ++common;
            height[rank[i]] = common;
        }

        sparse_table.assign(log_table[n] + 1, vector<int>(n));
        sparse_table[0] = height;
        for (int level = 1; level <= log_table[n]; ++level)
            for (int i = 0; i + (1 << level) <= n; ++i)
                sparse_table[level][i] =
                    min(sparse_table[level - 1][i],
                        sparse_table[level - 1][i + (1 << (level - 1))]);
    }

    int lcp(int first_suffix, int second_suffix) const {
        if (first_suffix == second_suffix) return int(text.size()) - first_suffix;
        int left_rank = rank[first_suffix], right_rank = rank[second_suffix];
        if (left_rank > right_rank) swap(left_rank, right_rank);
        ++left_rank;
        int level = log_table[right_rank - left_rank + 1];
        return min(sparse_table[level][left_rank],
                   sparse_table[level][right_rank - (1 << level) + 1]);
    }
};

// Usage:
// SuffixArray suffix_array(s);
// int common = suffix_array.lcp(x, y);
