#include <bits/stdc++.h>

#define V vector

typedef unsigned long long ull;

using namespace std;

/*
存 64 位无符号整数，可重复插入。erase(x) 只删一份，找不到 x 时返回 false。
max_xor(x, ans) 把最大异或值写入 ans，集合为空时返回 false。
每次操作走 64 位，删除时保留节点。
*/

struct XorTrie {
    struct Node {
        array < int, 2 > ch{};
        int cnt = 0;
    };
    V < Node > tr;

    XorTrie() { init(); }
    void init() { tr.assign(1, {}); }

    void insert(ull x) {
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

    bool erase(ull x) {
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

    bool max_xor(ull x, ull &ans) {
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

/*
XorTrie trie; trie.insert(x); ull ans;
bool ok = trie.max_xor(y, ans);
*/
