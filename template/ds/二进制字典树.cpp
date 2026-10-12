#include <bits/stdc++.h>

#define V vector

using namespace std;

// 64 位无符号整数的 01 Trie，支持重数和删除；节点编号从 0 开始。
// 单次操作 O(64)，空间 O(64 * 插入次数)，删除不回收节点。
// max_xor(x, ans) 成功时将集合元素与 x 的最大异或值写入 ans。
// 空集合返回 false，成功返回 true，此时才能使用 ans。

struct XorTrie {
    struct Node { array < int, 2 > ch{}; int cnt = 0; };
    V < Node > tr;

    XorTrie() { init(); }
    void init() { tr.assign(1, {}); }

    void insert(unsigned long long x) {
        int u = 0;
        tr[u].cnt ++;
        for (int i = 63; i >= 0; -- i) {
            int b = int(x >> i & 1);
            if (! tr[u].ch[b]) {
                tr[u].ch[b] = int(tr.size());
                tr.push_back({});
            }
            u = tr[u].ch[b]; tr[u].cnt ++;
        }
    }

    bool erase(unsigned long long x) {
        int u = 0;
        array < int, 65 > path{};
        if (! tr[u].cnt) return 0;
        for (int i = 63; i >= 0; -- i) {
            u = tr[u].ch[int(x >> i & 1)];
            if (! u || ! tr[u].cnt) return 0;
            path[64 - i] = u;
        }
        for (int p : path) tr[p].cnt --;
        return 1;
    }

    bool max_xor(unsigned long long x, unsigned long long &ans) {
        if (! tr[0].cnt) return false;
        int u = 0;
        ans = 0;
        for (int i = 63; i >= 0; -- i) {
            int b = int(x >> i & 1), v = tr[u].ch[b ^ 1];
            if (v && tr[v].cnt) ans |= 1ull << i, u = v;
            else u = tr[u].ch[b];
        }
        return true;
    }
};

// 调用示例：XorTrie trie; trie.insert(x); unsigned long long ans;
// bool ok = trie.max_xor(y, ans);
