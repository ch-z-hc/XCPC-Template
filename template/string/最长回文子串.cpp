// 回文半径及区间回文判断，0-based。预处理 O(n)，区间查询 O(1)，遍历找最长回文 O(n)。

#include <bits/stdc++.h>

#define V vector
#define se second
#define pii pair < int, int >
using namespace std;

struct Manacher {
    string s;
    V < int > odd, even;

    Manacher() = default;
    Manacher(const string &s) { build(s); }

    void build(const string &str) {
        s = str; int n = int(s.size());
        auto scan = [&](int gap) {
            V < int > radius(n);
            for (int i = 0, l = 0, r = -1; i < n; i ++) {
                int &k = radius[i];
                k = i > r ? 1 - gap : min(radius[l + r - i + gap], r - i + 1);
                while (i - k - gap >= 0 && i + k < n &&
                       s[i - k - gap] == s[i + k])
                    k ++;
                if (i + k - 1 > r) {
                    l = i - k + 1 - gap; r = i + k - 1;
                }
            }
            return radius;
        };
        odd = scan(0); even = scan(1);
    }

    bool query(int l, int r) {
        if (l > r) return true;
        int len = r - l + 1;
        if (len & 1) return odd[(l + r) / 2] >= len / 2 + 1;
        return even[(l + r + 1) / 2] >= len / 2;
    }

    pii longest() {
        pii ans{0, 0};
        for (int i = 0; i < int(s.size()); i ++) {
            int len1 = 2 * odd[i] - 1;
            if (len1 > ans.se) ans = {i - odd[i] + 1, len1};
            int len2 = 2 * even[i];
            if (len2 > ans.se) ans = {i - even[i], len2};
        }
        return ans;
    }
};

/*
Manacher man; man.build("abba");
bool pal = man.query(0, 3); auto [start, len] = man.longest();
*/
