#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
query(l, r) 判断 s[l..r] 是否回文，下标从 0 数。longest() 返回 {起点, 长度}。
odd[i] 是含中心 i 的半径，even[i] 的中心在 i - 1 和 i 之间。建表 O(n)，查询区间 O(1)。
*/

struct Manacher {
    string s;
    V < int > odd, even;

    Manacher() = default;
    Manacher(const string &s) { build(s); }

    void build(const string &str) {
        s = str;
        int n = int(s.size());
        odd.assign(n, 0);
        for (int i = 0, l = 0, r = -1; i < n; ++ i) {
            int k = i > r ? 1 : min(odd[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n &&
                   s[i - k] == s[i + k])
                ++ k;
            odd[i] = k;
            if (i + k - 1 > r) {
                l = i - k + 1;
                r = i + k - 1;
            }
        }

        even.assign(n, 0);
        for (int i = 0, l = 0, r = -1; i < n; ++ i) {
            int k = i > r ? 0 : min(even[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n &&
                   s[i - k - 1] == s[i + k])
                ++ k;
            even[i] = k;
            if (i + k - 1 > r) {
                l = i - k;
                r = i + k - 1;
            }
        }
    }

    bool query(int l, int r) {
        if (l > r) return true;
        int len = r - l + 1;
        if (len & 1) return odd[(l + r) / 2] >= len / 2 + 1;
        return even[(l + r + 1) / 2] >= len / 2;
    }

    pair < int, int > longest() {
        pair < int, int > ans{0, 0};
        for (int i = 0; i < int(s.size()); ++ i) {
            int len1 = 2 * odd[i] - 1;
            if (len1 > ans.second) ans = {i - odd[i] + 1, len1};
            int len2 = 2 * even[i];
            if (len2 > ans.second) ans = {i - even[i], len2};
        }
        return ans;
    }
};

/*
Manacher man(s);
bool pal = man.query(l, r);
*/
