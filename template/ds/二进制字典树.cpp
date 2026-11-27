/*
维护可重复的 64 位无符号整数，查询与 x 的最大异或值。每次操作 O(64)，删除不回收节点。
erase 每次只删一份，空集查询 max_xor 返回 false。
*/

#include <bits/stdc++.h>

#define V vector
typedef unsigned long long ull;
using namespace std;

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
        for (int i = 63; i >= 0; i --) {
            int b = int(x >> i & 1);
            if (!tr[u].ch[b]) {
                tr[u].ch[b] = int(tr.size());
                tr.push_back({});
            }
            u = tr[u].ch[b];
            tr[u].cnt ++;
        }
    }

    bool erase(ull x) {
        int u = 0;
        array < int, 65 > path{};
        if (!tr[u].cnt) return 0;
        for (int i = 63; i >= 0; i --) {
            u = tr[u].ch[int(x >> i & 1)];
            if (!u || !tr[u].cnt) return 0;
            path[64 - i] = u;
        }
        for (int p : path) tr[p].cnt --;
        return 1;
    }

    bool max_xor(ull x, ull &ans) {
        if (!tr[0].cnt) return false;
        int u = 0;
        ans = 0;
        for (int i = 63; i >= 0; i --) {
            int b = int(x >> i & 1), v = tr[u].ch[b ^ 1];
            if (v && tr[v].cnt)
                ans |= 1ull << i, u = v;
            else
                u = tr[u].ch[b];
        }
        return true;
    }
};

/*
XorTrie trie; trie.insert(5); ull ans;
if (trie.max_xor(2, ans)) cout << ans;
bool erased = trie.erase(5);
trie.init();
*/
