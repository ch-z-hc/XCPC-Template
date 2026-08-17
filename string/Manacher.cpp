#include <bits/stdc++.h>
using namespace std;

// Manacher 回文串；字符串下标 0-based。build O(n)，区间回文查询 O(1)。
// odd[i] 含中心字符，even[i] 的中心位于 i-1 和 i 之间。
struct Manacher {
    string text;
    vector<int> odd, even;

    Manacher() = default;
    explicit Manacher(const string &s) { build(s); }

    void build(const string &s) {
        text = s;
        int n = int(text.size());
        odd.assign(n, 0);
        for (int i = 0, left = 0, right = -1; i < n; ++i) {
            int radius = i > right ? 1 : min(odd[left + right - i], right - i + 1);
            while (i - radius >= 0 && i + radius < n &&
                   text[i - radius] == text[i + radius])
                ++radius;
            odd[i] = radius;
            if (i + radius - 1 > right) {
                left = i - radius + 1;
                right = i + radius - 1;
            }
        }

        even.assign(n, 0);
        for (int i = 0, left = 0, right = -1; i < n; ++i) {
            int radius = i > right ? 0 : min(even[left + right - i + 1], right - i + 1);
            while (i - radius - 1 >= 0 && i + radius < n &&
                   text[i - radius - 1] == text[i + radius])
                ++radius;
            even[i] = radius;
            if (i + radius - 1 > right) {
                left = i - radius;
                right = i + radius - 1;
            }
        }
    }

    bool is_palindrome(int left, int right) const {
        if (left > right) return true;
        int length = right - left + 1;
        if (length & 1) return odd[(left + right) / 2] >= length / 2 + 1;
        return even[(left + right + 1) / 2] >= length / 2;
    }

    pair<int, int> longest_palindrome() const {
        pair<int, int> answer{0, 0};
        for (int i = 0; i < int(text.size()); ++i) {
            int odd_length = 2 * odd[i] - 1;
            if (odd_length > answer.second) answer = {i - odd[i] + 1, odd_length};
            int even_length = 2 * even[i];
            if (even_length > answer.second) answer = {i - even[i], even_length};
        }
        return answer;
    }
};

// Usage:
// Manacher manacher(s);
// bool palindrome = manacher.is_palindrome(left, right);
